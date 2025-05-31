#include "compiler.h"
#include "common/common.h"
#include "scanner/scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef void (*ParseFn)(ZBool canAssign);

static ZUInt8 identifierConstant(Token* name);

//ZIA's precedence order from lowest to the highest
typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_CONDITIONAL, // ?:
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
}Precedence;

typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
}ParseRule;

typedef struct
{
    Token name;
    ZInt32 depth;
}Local;

typedef enum
{
    TYPE_FUNCTION,
    TYPE_SCRIPT
}FunctionType;

typedef struct
{
    ObjFunction* function;
    FunctionType type;

    Local locals[UINT8_COUNT];
    ZInt32 localCount;
    ZInt32 scopeDepth;
}Compiler;


typedef struct
{
    Token current;
    Token previous;
    ZBool hadError;
    ZBool panicMode;
}Parser;

Parser parser;
Compiler* current = NULL;

static Chunk* currentChunk()
{
    return &current->function->chunk;
}

static void errorAt(Token* token, const ZChar* message)
{
    if (parser.panicMode)
    {
       return;
    }
    
    parser.panicMode = true;
    fprintf(stderr, "[ligne %d] Erreur", token->line);
    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " à la fin");
    }
    else if (token->type == TOKEN_ERROR)
    {
        /* code */
    }
    else
    {
        fprintf(stderr, " à '%.*s'", token->length, token->start);
    }
    fprintf(stderr, " : %s\n", message);
    parser.hadError = true;
}


static void error(const ZChar* message)
{
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const ZChar* message)
{
    errorAt(&parser.current, message);
}

static void advance()
{
    parser.previous = parser.current;

    for(;;)
    {
        parser.current =  scanToken();
        if (parser.current.type != TOKEN_ERROR)
        {
            break;
        }
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const ZChar* message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }
    errorAtCurrent(message);
}

static ZBool check(TokenType type)
{
    return parser.current.type == type;
}

static ZBool match(TokenType type){
    if (!check(type))
    {
        return ZFALSE;
    }

    advance();

    return ZTRUE;
}

static void emitByte(ZUInt8 byte)
{
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(ZUInt8 byte1, ZUInt8 byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

static void emitLoop(ZInt32 loopStart)
{
    emitByte(OP_LOOP);

    ZInt32 offset = currentChunk()->count - loopStart + 2;
    if (offset > UINT16_MAX)
    {
        error("Corps de boucle trop long");
    }
    
    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

static ZInt32 emitJump(ZUInt8 instruction)
{
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return currentChunk()->count - 2;
}

static void emitReturn()
{
    emitByte(OP_RETURN);
}

static ZUInt8 makeConstant(Value value)
{
    ZInt32 constant =  addConstant(currentChunk(), value);
    if (constant > UINT8_MAX)
    {
        error("Trop de constantes dans un seul bloc.");
        return 0;
    }
    return (ZUInt8)constant;
}

static void emitConstant(Value value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static void patchJump(ZInt32 offset)
{
    ZInt32 jump = currentChunk()->count - offset - 2;

    if (jump > UINT16_MAX)
    {
        error("Trop de code à sauter.");
    }
    
    currentChunk()->code[offset] = (jump >> 8) & 0xff;
    currentChunk()->code[offset + 1] = jump & 0xff;
}

static void initCompiler(Compiler* compiler, FunctionType type)
{
    compiler->function = NULL;
    compiler->type = type;

    compiler->localCount = 0;
    compiler->scopeDepth = 0;

    compiler->function = newFunction();

    current = compiler;

    Local* local = &current->locals[current->localCount++];
    local->depth = 0;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction* endCompiler()
{
    emitReturn();
    ObjFunction* function = current->function;

#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError)
    {
        disassembleChunk(currentChunk(), NULL != function->name ? function->name->chars : "<script>");
    }
#endif

    return function;
}

static void beginScope()
{
    current->scopeDepth++;
}

static void endScope()
{
    current->scopeDepth--;

    while (current->localCount > 0 && 
          (current->locals[current->localCount - 1].depth > current->scopeDepth))
    {
        emitByte(OP_POP);
        current->localCount--;
    }
    
}

static void expression();
static void statement();
static void declaration();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
static int resolveLocal(Compiler* compiler, Token* name);

static void binary(ZBool canAssign)
{
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType)
    {
    case TOKEN_BANG_EQUAL:
        emitBytes(OP_EQUAL, OP_NOT);
        break;
    case TOKEN_EQUAL_EQUAL:
        emitByte(OP_EQUAL);
        break;
    case TOKEN_GREATER:
        emitByte(OP_GREATER);
        break;
    case TOKEN_GREATER_EQUAL:
        emitBytes(OP_LESS, OP_NOT);
        break;
    case TOKEN_LESS:
        emitByte(OP_LESS);
        break;
    case TOKEN_LESS_EQUAL:
        emitBytes(OP_GREATER, OP_NOT);
        break;
    case TOKEN_PLUS:
        emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emitByte(OP_DIVIDE);
        break;
    case TOKEN_PERCENT:
        emitByte(OP_MODULO);
        break;
    default:
        return;
    }
}

static void literal(ZBool canAssign)
{
    switch (parser.previous.type)
    {
    case TOKEN_FALSE:
        emitByte(OP_FALSE);
        break;
    case TOKEN_TRUE:
        emitByte(OP_TRUE);
        break;
    case TOKEN_NULL:
        emitByte(OP_NULL);
        break;
    default:
        return;
    }
}

static void conditional(ZBool canAssign) {

    // Parse true branch
    ZInt32 thenJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP); // Pop condition if true

    parsePrecedence(PREC_CONDITIONAL);

    ZInt32 elseJump = emitJump(OP_JUMP);

    patchJump(thenJump);
    emitByte(OP_POP); // Pop condition if false

    consume(TOKEN_COLON, "Un ':' est attendu après '?' dans l'expression ternaire.");

    parsePrecedence(PREC_CONDITIONAL);

    patchJump(elseJump);
  }

static void grouping(ZBool canAssign)
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Parenthèse fermante ')' attendue après l'expression.");
}

static void number(ZBool canAssign)
{
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void and_(ZBool canAssign)
{
    ZInt32 endJump = emitJump(OP_JUMP_IF_FALSE);

    emitByte(OP_POP);
    parsePrecedence(PREC_AND);

    patchJump(endJump);
}

static void or_(ZBool canAssign)
{
    ZInt32 elseJump = emitJump(OP_JUMP_IF_FALSE);
    ZInt32 endJump = emitJump(OP_JUMP);

    patchJump(elseJump);
    emitByte(OP_POP);

    parsePrecedence(PREC_OR);
    patchJump(endJump);
}

static void string(ZBool canAssign)
{
    //@TBD:  string escape sequences like \n, we’d translate those here
    emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

static void namedVariable(Token name, ZBool canAssign)
{
    ZUInt8 getOp, setOp;
    ZInt32 arg = resolveLocal(current, &name);
    
    if (arg != -1)
    {
        getOp = OP_GET_LOCAL;
        setOp = OP_SET_LOCAL;
    }
    else
    {
        arg = identifierConstant(&name);
        getOp = OP_GET_GLOBAL;
        setOp = OP_SET_GLOBAL;
    }

    if (canAssign && match(TOKEN_EQUAL))
    {
        expression();
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_PLUS_PLUS))
    {
        namedVariable(name, ZFALSE);
        emitByte(OP_INCREMENT);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_MINUS_MINUS))
    {
        namedVariable(name, ZFALSE);
        emitByte(OP_DECREMENT);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_PLUS_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_ADD);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_MINUS_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_SUBTRACT);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_STAR_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_MULTIPLY);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if(canAssign && match(TOKEN_SLASH_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_DIVIDE);
        emitBytes(setOp, (ZUInt8)arg);
    }

    else
    {
        emitBytes(getOp, (ZUInt8)arg);
    }  
}

static void variable(ZBool canAssign)
{
    namedVariable(parser.previous, canAssign);
}

static void unary(ZBool canAssign)
{
    TokenType operatorType = parser.previous.type;

    parsePrecedence(PREC_UNARY);

    switch (operatorType)
    {
    case TOKEN_BANG:
        emitByte(OP_NOT);
        break;
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return;
    }
}

static void increment(bool canAssign) {
    emitByte(OP_INCREMENT);
}

static void decrement(bool canAssign) {
    emitByte(OP_DECREMENT);
}

ParseRule rules[] = 
{
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,        PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,        PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,        PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,        PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,        PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary,      PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary,      PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,        PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary,      PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary,      PREC_FACTOR},
    [TOKEN_BANG]          = {unary,    NULL,        PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary,      PREC_EQUALITY},
    [TOKEN_EQUAL]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary,      PREC_EQUALITY},
    [TOKEN_GREATER]       = {NULL,     binary,      PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary,      PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary,      PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary,      PREC_COMPARISON},
    [TOKEN_IDENTIFIER]    = {variable, NULL,        PREC_NONE},
    [TOKEN_STRING]        = {string,   NULL,        PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,        PREC_NONE},
    [TOKEN_AND]           = {NULL,     and_,        PREC_AND},
    [TOKEN_CLASS]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,        PREC_NONE},
    [TOKEN_FALSE]         = {literal,  NULL,        PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,        PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,        PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,        PREC_NONE},
    [TOKEN_NULL]          = {literal,  NULL,        PREC_NONE},
    [TOKEN_OR]            = {NULL,     or_,         PREC_OR},
    [TOKEN_PRINT]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,        PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,        PREC_NONE},
    [TOKEN_TRUE]          = {literal,  NULL,        PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,        PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,        PREC_NONE},

    [TOKEN_COLON]         = {NULL,     NULL,        PREC_NONE},
    [TOKEN_QUESTION]      = {NULL,     conditional, PREC_CONDITIONAL},
    [TOKEN_PERCENT]       = {NULL,     binary,      PREC_FACTOR},
    [TOKEN_PLUS_PLUS]     = {NULL,     increment,   PREC_NONE},
    [TOKEN_PLUS_EQUAL]    = {NULL,     NULL,        PREC_NONE},
    [TOKEN_MINUS_MINUS]   = {NULL,     decrement,   PREC_NONE},
    [TOKEN_MINUS_EQUAL]   = {NULL,     NULL,        PREC_NONE},
    [TOKEN_STAR_EQUAL]    = {NULL,     NULL,        PREC_NONE},
    [TOKEN_SLASH_EQUAL]   = {NULL,     NULL,        PREC_NONE},

    [TOKEN_EOF]           = {NULL,     NULL,        PREC_NONE},

};

static void parsePrecedence(Precedence precedence)
{
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;

    if (NULL == prefixRule)
    {
        //it might be a syntax error
        error("Expression syntaxiquement incorrecte.");
        return;
    }

    ZBool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign);

    while (precedence <= getRule(parser.current.type)->precedence)
    {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }
    
    if (canAssign && match(TOKEN_EQUAL))
    {
        error("Impossible d’assigner une valeur ici.");
    }
    
}

static ZUInt8 identifierConstant(Token* name)
{
    return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

static ZBool identifiersEqual(Token* a, Token* b)
{
    if (a->length != b->length)
    {
        return ZFALSE;
    }
    return memcmp(a->start, b->start, a->length) == 0;
}
static int resolveLocal(Compiler* compiler, Token* name)
{
    for (ZInt32 i = compiler->localCount - 1; i >= 0; i--)
    {
        Local* local = &compiler->locals[i];
        if (identifiersEqual(name, &local->name))
        {
            if (local->depth == -1)
            {
                error("impossible de lire une variable locale dans sa propre initialisation.");
            }
            
            return i;
        }
    }
    
    return - 1;
}

static void addLocal(Token name)
{
    if (current->localCount == UINT8_COUNT)
    {
        //TBD: extend 256 local vairable number in a scope to four byte
        error("Trop de variables locales dans un seul bloc.");
        return;
    }
    
    Local* local = &current->locals[current->localCount++];
    local->name = name;
    local->depth = -1;
}

static void declareVariable()
{
    if (current->scopeDepth == 0)
    {
        return;
    }
    
    Token* name = &parser.previous;
    for(ZInt32 i = current->localCount - 1; i >= 0; i--)
    {
        Local* local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth)
        {
            break;
        }
        if (identifiersEqual(name, &local->name))
        {
            error("Une variable avec ce nom existe déjà dans cette portée");
        }
        
    }
    addLocal(*name);
}

static ZUInt8 parseVariable(const ZChar* errorMessage)
{
    consume(TOKEN_IDENTIFIER, errorMessage);

    declareVariable();
    if (current->scopeDepth > 0)
    {
        return 0;
    }
    

    return identifierConstant(&parser.previous);
}

static void markInitialized()
{
    current->locals[current->localCount - 1].depth = current->scopeDepth;
}

static void defineVariable(ZUInt8 global)
{
    if (current->scopeDepth > 0)
    {
        markInitialized();
        return;
    }
    
    emitBytes(OP_DEFINE_GLOBAL, global);
}

static ParseRule* getRule(TokenType type)
{
    return &rules[type];
}

static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}

static void block()
{
    while(!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF))
    {
        declaration();
    }


    consume(TOKEN_RIGHT_BRACE, "Un '}' est attendu après le bloc.");
}

static void varDeclaration()
{
    ZUInt8 global = parseVariable("Nom de variable attendu.");

    if (match(TOKEN_EQUAL))
    {
        expression();
    }
    else
    {
        //if the user does not initialize a variable zia compiler should initialize it to nul.
        emitByte(OP_NULL);
    }

    consume(TOKEN_SEMICOLON, "Point-virgule attendu après la déclaration de variable.");
    defineVariable(global);
}

static void expressionStatement()
{
    expression();
    consume(TOKEN_SEMICOLON, "Erreur : point-virgule manquant après la valeur.");
    emitByte(OP_POP);
}

static void forStatement()
{
    beginScope();
    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après boucle 'pour'.");

    if (match(TOKEN_SEMICOLON))
    {
        /*
        No initializer found ex.: for (;i>10;i++)
        */
    }
    else if(match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        expressionStatement();
    }
    
    ZInt32 loopStart = currentChunk()->count;
    ZInt32 exitJump = -1;
    
    if (!match(TOKEN_SEMICOLON))
    {
        expression();
        consume(TOKEN_SEMICOLON, "Point-virgule ';' attendu après la condition de la boucle 'pour'.");

        exitJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP);
    }

    if (!match(TOKEN_RIGHT_PAREN))
    {
        ZInt32 bodyJump = emitJump(OP_JUMP);
        ZInt32 incrementStart = currentChunk()->count;
        expression();
        emitByte(OP_POP);
        consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après les clauses du boucle 'pour'.");

        emitLoop(loopStart);
        loopStart = incrementStart;
        patchJump(bodyJump);
    }
    
    statement();
    emitLoop(loopStart);

    /*
    We do this only when there is a condition clause. If there isn’t, 
    there’s no jump to patch and no condition value on the stack to pop.
    */
    if (exitJump != -1)
    {
        patchJump(exitJump);
        emitByte(OP_POP);
    }

    endScope();
}

static void ifStatement() {
    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après 'si'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après la condition.");

    ZInt32 thenJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);   // Pop condition if true
    statement();

    ZInt32 endJump = emitJump(OP_JUMP);
    patchJump(thenJump);
    emitByte(OP_POP);   // Pop condition if false

    while (match(TOKEN_ELSE_IF)) {  // Handle multiple "sinon si"
        consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après 'sinon si'.");
        expression();
        consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après la condition.");

        ZInt32 elseifJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP);   // Pop condition if true
        statement();

        ZInt32 nextJump = emitJump(OP_JUMP);
        patchJump(elseifJump);
        emitByte(OP_POP);   // Pop condition if false

        endJump = nextJump;
    }

    if (match(TOKEN_ELSE)) {
        statement();
    }

    patchJump(endJump);
}

static void printStatement()
{
    expression();
    consume(TOKEN_SEMICOLON, "Erreur : point-virgule manquant après la valeur.");
    emitByte(OP_PRINT);
}

static void whileStatement()
{
    ZInt32 loopStart = currentChunk()->count;

    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après boucle 'tantque'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après une condition.");

    ZInt32 exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);
    statement();
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(OP_POP);
}

static void synchronize()
{
    parser.panicMode = ZFALSE;

    while (parser.current.type != TOKEN_EOF)
    {
        if (parser.previous.type == TOKEN_SEMICOLON)
        {
            return;
        }
        switch (parser.current.type)
        {
        case TOKEN_CLASS:
        case TOKEN_FUN:
        case TOKEN_VAR:
        case TOKEN_FOR:
        case TOKEN_IF:
        case TOKEN_WHILE:
        case TOKEN_PRINT:
        case TOKEN_RETURN:
            return;
        default:
            ;
        }
         
        advance();
    }
    
}

static void declaration()
{
    if (match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        statement();
    }

    if (parser.panicMode)
    {
        synchronize();
    }
    
}

static void statement()
{
    if (match(TOKEN_PRINT))
    {
        printStatement();
    }
    else if(match(TOKEN_FOR))
    {
        forStatement();
    }
    else if(match(TOKEN_IF))
    {
        ifStatement();
    }
    else if(match(TOKEN_WHILE))
    {
        whileStatement();
    }
    else if(match(TOKEN_LEFT_BRACE))
    {
        beginScope();
        block();
        endScope();
    }
    else
    {
        expressionStatement();
    }
}

ObjFunction* compile(const ZChar* source)
{
    initScanner(source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_SCRIPT);

    parser.hadError = false;
    parser.panicMode = false;

    advance();

    while(!match(TOKEN_EOF))
    {
        declaration();
    }

    ObjFunction* function = endCompiler();
    /*
    @NOTE: This way, the VM doesn’t try to execute a function that may contain invalid bytecode.
    */
    return parser.hadError ? NULL : function;
}