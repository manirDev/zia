#include "compiler.h"
#include "common/common.h"
#include "scanner/scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "memory/memory.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

#define MAX_NESTED_LOOPS 10
#define MAX_CASES 10
#define MAX_ARGS 255

typedef void (*ParseFn)(ZBool canAssign);

static ZUInt8 identifierConstant(Token *name);
static void postIncrementDecrement(ZUInt8 getOp, ZUInt8 setOp, ZInt32 arg, ZUInt8 operation);
static void synchronize();
static ZUInt8 argumentList();

// ZIA's precedence order from lowest to the highest
typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT,  // =, +=, -=, /=, *=
    PREC_CONDITIONAL, // ?:
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -, ++, --,
    PREC_POWER,       // **
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

typedef struct
{
    Token name;
    ZInt32 depth;
    ZBool isCaptured;
} Local;

typedef struct
{
    ZUInt8 index;
    ZBool isLocal;
} Upvalue;

typedef enum
{
    TYPE_FUNCTION,
    TYPE_SCRIPT
} FunctionType;

typedef struct
{
    ZInt32 *breakJumps;   // for break jump locations
    ZInt32 breakCount;    // number of active breaks
    ZInt32 breakCapacity; // capacity of breakJumps array

    ZInt32 loopStart;
    ZInt32 loopEnd;        // track where the break should jump to
    ZInt32 scopeDepth;     // track the scope depth when loop started
    ZInt32 incrementStart; // where continue should jump to (for for loops)
} Loop;

typedef struct
{
    ZInt32 *switchBreakJumps;
    ZInt32 switchBreakCount;
    ZInt32 switchDepth;
} SwitchContext;

typedef struct
{
    // support up to MAX_NESTED_LOOPS nested loop;
    Loop loops[MAX_NESTED_LOOPS];
    ZInt32 loopDepth;
} LoopContext;

typedef struct Compiler
{
    struct Compiler *enclosing;
    ObjFunction *function;
    FunctionType type;
    Local locals[UINT8_COUNT];
    ZInt32 localCount;
    Upvalue upvalues[UINT8_COUNT];
    ZInt32 scopeDepth;
    LoopContext loopContext;
    SwitchContext switchContext;
} Compiler;

typedef struct
{
    Token current;
    Token previous;
    ZBool hadError;
    ZBool panicMode;
} Parser;

Parser parser;
Compiler *current = NULL;

static Chunk *currentChunk()
{
    return &current->function->chunk;
}

static void errorAt(Token *token, const ZChar *message)
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

static void error(const ZChar *message)
{
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const ZChar *format, ...)
{
    ZChar message[256];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof message, format, args);
    va_end(args);

    errorAt(&parser.current, message);
}

static void advance()
{
    parser.previous = parser.current;

    for (;;)
    {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)
        {
            break;
        }
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const ZChar *message)
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

static ZBool match(TokenType type)
{
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

    ZInt32 offset = currentChunk()->count - loopStart + JUMP_OFFSET_SIZE;
    if (offset > 0xFFFFFF)
    {
        error("Corps de boucle trop long");
    }

    emitByte((offset >> 16) & 0xff);
    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

static ZInt32 emitJump(ZUInt8 instruction)
{
    emitByte(instruction);
    emitByte(0xff); // High byte
    emitByte(0xff); // Middle byte
    emitByte(0xff); // Low byte
    return currentChunk()->count - JUMP_OFFSET_SIZE;
}

static void emitReturn()
{
    emitByte(OP_NULL);
    emitByte(OP_RETURN);
}

static ZUInt8 makeConstant(Value value)
{
    ZInt32 constant = addConstant(currentChunk(), value);
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
    ZInt32 jump = currentChunk()->count - offset - JUMP_OFFSET_SIZE;
    if (jump > 0xFFFFFF)
    {
        error("Jump offset too large.");
    }
    currentChunk()->code[offset] = (jump >> 16) & 0xff;
    currentChunk()->code[offset + 1] = (jump >> 8) & 0xff;
    currentChunk()->code[offset + 2] = jump & 0xff;
}

static void initCompiler(Compiler *compiler, FunctionType type)
{
    compiler->enclosing = current;
    compiler->function = NULL;
    compiler->type = type;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;

    // Initialize loop context
    compiler->loopContext.loopDepth = 0;
    for (int i = 0; i < MAX_NESTED_LOOPS; i++)
    { // Initialize all potential loop levels
        compiler->loopContext.loops[i].breakJumps = NULL;
        compiler->loopContext.loops[i].breakCount = 0;
        compiler->loopContext.loops[i].breakCapacity = 0;
        compiler->loopContext.loops[i].loopStart = 0;
        compiler->loopContext.loops[i].loopEnd = 0;
        compiler->loopContext.loops[i].incrementStart = 0;
    }

    compiler->switchContext.switchDepth = 0;
    compiler->switchContext.switchBreakJumps = NULL;
    compiler->switchContext.switchBreakCount = 0;

    compiler->function = newFunction();
    current = compiler;
    if (TYPE_SCRIPT != type)
    {
        current->function->name = copyString(parser.previous.start, parser.previous.length);
    }

    // Initialize first local slot
    Local *local = &current->locals[current->localCount++];
    local->depth = 0;
    local->isCaptured = ZFALSE;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction *endCompiler()
{
    // Clean up all loop tracking resources
    for (ZInt32 i = 0; i < current->loopContext.loopDepth; i++)
    {
        if (current->loopContext.loops[i].breakJumps != NULL)
        {
            free(current->loopContext.loops[i].breakJumps);
            current->loopContext.loops[i].breakJumps = NULL;
        }
        current->loopContext.loops[i].breakCount = 0;
        current->loopContext.loops[i].breakCapacity = 0;
        current->loopContext.loops[i].incrementStart = 0;
    }
    current->loopContext.loopDepth = 0;

    // FREE_ARRAY(ZInt32, current->switchContext.switchBreakJumps, current->switchContext.switchBreakCount);

    emitReturn();
    ObjFunction *function = current->function;

#ifdef DEBUG_PRINT_CODE
    if (ZTRUE == FLAG_PRINT_CODE)
    {
        if (!parser.hadError)
        {
            disassembleChunk(currentChunk(),
                             function->name != NULL ? function->name->chars : "<script>");
        }
    }
#endif

    /*when a Compiler finishes, it pops itself off the stack by restoring
    the previous compiler to be the new current one.*/
    current = current->enclosing;
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
        if (ZTRUE == current->locals[current->localCount - 1].isCaptured)
        {
            emitByte(OP_CLOSE_UPVALUE);
        }
        else
        {
            emitByte(OP_POP);
        }
        current->localCount--;
    }
}

static void beginLoop()
{
    Compiler *c = current;
    if (c->loopContext.loopDepth >= MAX_NESTED_LOOPS)
    {
        error("Trop de boucles imbriquées");
        return;
    }

    Loop *loop = &c->loopContext.loops[c->loopContext.loopDepth++];
    loop->breakJumps = NULL;
    loop->breakCount = 0;
    loop->breakCapacity = 0;

    loop->loopStart = currentChunk()->count;
    loop->loopEnd = -1; // will be set when know that the loop ends
    loop->incrementStart = -1;
    loop->scopeDepth = c->scopeDepth; // Store current scope depth
}

static void endLoop()
{
    Compiler *c = current;
    if (c->loopContext.loopDepth == 0)
    {
        error("No active loop to end");
        return;
    }

    Loop *loop = &c->loopContext.loops[--c->loopContext.loopDepth];

    // Patch break jumps to loopEnd
    for (ZInt32 i = 0; i < loop->breakCount; i++)
    {
        patchJump(loop->breakJumps[i]);
    }

    // Free memory
    if (loop->breakJumps != NULL)
        free(loop->breakJumps);
}

static void addBreakJump(ZInt32 jump)
{
    Compiler *c = current;

    // ✅ Check if we are in a switch
    if (c->switchContext.switchDepth > 0)
    {
        if (c->switchContext.switchBreakJumps == NULL)
        {
            c->switchContext.switchBreakJumps = malloc(sizeof(ZInt32) * MAX_CASES);
            c->switchContext.switchBreakCount = 0;
        }

        if (c->switchContext.switchBreakCount < MAX_CASES)
        {
            c->switchContext.switchBreakJumps[c->switchContext.switchBreakCount++] = jump;
        }
        else
        {
            error("Trop de 'quitter' dans 'selon'.");
        }
        return;
    }

    // ✅ Otherwise fallback to loop support
    if (c->loopContext.loopDepth == 0)
    {
        error("Les instructions 'quitter' ne peuvent être utilisées que dans une boucle ou un 'selon'.");
        return;
    }

    Loop *loop = &c->loopContext.loops[c->loopContext.loopDepth - 1];

    if (loop->breakCount >= loop->breakCapacity)
    {
        ZInt32 newCapacity = loop->breakCapacity < MAX_NESTED_LOOPS ? MAX_NESTED_LOOPS : loop->breakCapacity * 2;
        loop->breakJumps = realloc(loop->breakJumps, sizeof(ZInt32) * newCapacity);
        loop->breakCapacity = newCapacity;
    }
    loop->breakJumps[loop->breakCount++] = jump;
}

static void expression();
static void statement();
static void declaration();
static ParseRule *getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
static int resolveLocal(Compiler *compiler, Token *name);
static ZInt32 resolveUpvalue(Compiler *compiler, Token *name);

static void binary(ZBool canAssign)
{
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
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
    case TOKEN_STAR_STAR:
        emitByte(OP_POWER);
        break;
    default:
        return;
    }
}

static void call(ZBool canAssign)
{
    ZUInt8 argCount = argumentList();
    emitBytes(OP_CALL, argCount);
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

static void conditional(ZBool canAssign)
{

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

static void string(bool _canAssign)
{
    // -2 because we ignore opening and closing quotes:'"'
    int origLength = parser.previous.length - 2;
    int escapedLength = 0;
    char *escapedStr = ALLOCATE(char, origLength);
    // loop all chars and combine '\'+'n' -> '\n' char, adjust total length:
    for (int i = 1; i < origLength + 1; ++i)
    {
        char c = parser.previous.start[i];
        if (i < origLength && c == '\\')
        {
            char nextChar = parser.previous.start[++i];
            switch (nextChar)
            {
            case '\n':
                break;
            case '\\':
                escapedStr[escapedLength++] = '\\';
                break;
            case '\'':
                escapedStr[escapedLength++] = '\'';
                break;
            case '\"':
                escapedStr[escapedLength++] = '\"';
                break;
            case 'n':
                escapedStr[escapedLength++] = '\n';
                break;
            case 't':
                escapedStr[escapedLength++] = '\t';
                break;
            default:
                break;
            }
            continue;
        }
        escapedStr[escapedLength++] = c;
    }
    emitConstant(OBJ_VAL(copyString(escapedStr, escapedLength)));
    FREE(char, escapedStr);
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
    else if ((arg = resolveUpvalue(current, &name)) != -1)
    {
        getOp = OP_GET_UPVALUE;
        setOp = OP_SET_UPVALUE;
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
    else if (canAssign && match(TOKEN_PLUS_PLUS_POSTFIX))
    {
        postIncrementDecrement(getOp, setOp, arg, OP_INCREMENT);
    }
    else if (canAssign && match(TOKEN_MINUS_MINUS_POSTFIX))
    {
        postIncrementDecrement(getOp, setOp, arg, OP_DECREMENT);
    }

    else if (canAssign && match(TOKEN_PLUS_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_ADD);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if (canAssign && match(TOKEN_MINUS_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_SUBTRACT);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if (canAssign && match(TOKEN_STAR_EQUAL))
    {
        namedVariable(name, ZFALSE);
        expression();
        emitByte(OP_MULTIPLY);
        emitBytes(setOp, (ZUInt8)arg);
    }
    else if (canAssign && match(TOKEN_SLASH_EQUAL))
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

static void postIncrementDecrement(ZUInt8 getOp, ZUInt8 setOp, ZInt32 arg, ZUInt8 operation)
{
    // Post-increment: a++
    // 1. Get original value
    emitBytes(getOp, (ZUInt8)arg);
    // 2. Duplicate it
    emitByte(OP_DUP);
    // 3. Increment the copy
    emitByte(operation);
    // 4. Store the incremented value back to variable
    emitBytes(setOp, (ZUInt8)arg);
    // Stack now has [original, incremented]
    // We want to keep original and discard incremented
    emitByte(OP_POP);
}
static void preIncrementDecrement(ZUInt8 operation)
{
    // We need to handle this similar to assignment
    // The previous token should be the ++ operator, and current should be the variable
    advance(); // Move to the variable token

    Token name = parser.previous;
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

    emitBytes(getOp, (ZUInt8)arg);
    emitByte(operation);
    emitByte(OP_DUP);
    emitBytes(setOp, (ZUInt8)arg);
}

static void preIncrement(bool canAssign)
{
    preIncrementDecrement(OP_INCREMENT);
}

static void preDecrement(bool canAssign)
{
    preIncrementDecrement(OP_DECREMENT);
}

static void postIncrement(bool canAssign)
{
    emitByte(OP_INCREMENT);
}

static void postDecrement(bool canAssign)
{
    emitByte(OP_DECREMENT);
}

ParseRule rules[] =
    {
        [TOKEN_LEFT_PAREN] = {grouping, call, PREC_CALL},
        [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
        [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
        [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
        [TOKEN_MINUS] = {unary, binary, PREC_TERM},
        [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
        [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
        [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
        [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
        [TOKEN_BANG] = {unary, NULL, PREC_NONE},
        [TOKEN_BANG_EQUAL] = {NULL, binary, PREC_EQUALITY},
        [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUAL_EQUAL] = {NULL, binary, PREC_EQUALITY},
        [TOKEN_GREATER] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_GREATER_EQUAL] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_LESS] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_LESS_EQUAL] = {NULL, binary, PREC_COMPARISON},
        [TOKEN_IDENTIFIER] = {variable, NULL, PREC_NONE},
        [TOKEN_STRING] = {string, NULL, PREC_NONE},
        [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
        [TOKEN_AND] = {NULL, and_, PREC_AND},
        [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
        [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
        [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
        [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
        [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
        [TOKEN_IF] = {NULL, NULL, PREC_NONE},
        [TOKEN_NULL] = {literal, NULL, PREC_NONE},
        [TOKEN_OR] = {NULL, or_, PREC_OR},
        [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
        [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
        [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
        [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
        [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
        [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
        [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
        [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},

        [TOKEN_COLON] = {NULL, NULL, PREC_NONE},
        [TOKEN_QUESTION] = {NULL, conditional, PREC_CONDITIONAL},
        [TOKEN_PERCENT] = {NULL, binary, PREC_FACTOR},
        [TOKEN_PLUS_PLUS_POSTFIX] = {NULL, postIncrement, PREC_NONE},
        [TOKEN_PLUS_PLUS_PREFIX] = {preIncrement, NULL, PREC_UNARY},
        [TOKEN_PLUS_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_MINUS_MINUS_POSTFIX] = {NULL, postDecrement, PREC_NONE},
        [TOKEN_MINUS_MINUS_PREFIX] = {preDecrement, NULL, PREC_UNARY},
        [TOKEN_MINUS_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_STAR_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_SLASH_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_STAR_STAR] = {NULL, binary, PREC_POWER},
        [TOKEN_SWITCH] = {NULL, NULL, PREC_NONE},
        [TOKEN_CASE] = {NULL, NULL, PREC_NONE},
        [TOKEN_DEFAULT] = {NULL, NULL, PREC_NONE},

        [TOKEN_EOF] = {NULL, NULL, PREC_NONE},

};

static void parsePrecedence(Precedence precedence)
{
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;

    if (NULL == prefixRule)
    {
        // it might be a syntax error
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

static ZUInt8 identifierConstant(Token *name)
{
    return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

static ZBool identifiersEqual(Token *a, Token *b)
{
    if (a->length != b->length)
    {
        return ZFALSE;
    }
    return memcmp(a->start, b->start, a->length) == 0;
}
static ZInt32 resolveLocal(Compiler *compiler, Token *name)
{
    for (ZInt32 i = compiler->localCount - 1; i >= 0; i--)
    {
        Local *local = &compiler->locals[i];
        if (identifiersEqual(name, &local->name))
        {
            if (local->depth == -1)
            {
                error("impossible de lire une variable locale dans sa propre initialisation.");
            }

            return i;
        }
    }

    return -1;
}

static ZInt32 addUpvalue(Compiler *compiler, ZUInt8 index, ZBool isLocal)
{
    ZInt32 upvalueCount = compiler->function->upvalueCount;

    for (ZInt32 i = 0; i < upvalueCount; i++)
    {
        Upvalue *upvalue = &compiler->upvalues[i];
        if (upvalue->index == index && upvalue->isLocal == isLocal)
        {
            return i;
        }
    }

    if (UINT8_COUNT == upvalueCount)
    {
        error("Trop de variables capturées dans la fonction.");
        return 0;
    }

    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = index;
    return compiler->function->upvalueCount++;
}

static ZInt32 resolveUpvalue(Compiler *compiler, Token *name)
{
    if (NULL == compiler->enclosing)
    {
        return -1;
    }

    ZInt32 local = resolveLocal(compiler->enclosing, name);
    if (local != -1)
    {
        compiler->enclosing->locals[local].isCaptured = ZTRUE;
        return addUpvalue(compiler, (ZUInt8)local, ZTRUE);
    }

    ZInt32 upvalue = resolveUpvalue(compiler->enclosing, name);
    if (upvalue != -1)
    {
        return addUpvalue(compiler, (ZUInt8)upvalue, ZFALSE);
    }

    return -1;
}

static void addLocal(Token name)
{
    if (current->localCount == UINT8_COUNT)
    {
        // TBD: extend 256 local vairable number in a scope to four byte
        error("Trop de variables locales dans un seul bloc.");
        return;
    }

    Local *local = &current->locals[current->localCount++];
    local->name = name;
    local->depth = -1;
    local->isCaptured = ZFALSE;
}

static void declareVariable()
{
    if (current->scopeDepth == 0)
    {
        return;
    }

    Token *name = &parser.previous;
    for (ZInt32 i = current->localCount - 1; i >= 0; i--)
    {
        Local *local = &current->locals[i];
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

static ZUInt8 parseVariable(const ZChar *errorMessage)
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
    if (current->scopeDepth == 0)
    {
        return;
    }

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

static ZUInt8 argumentList()
{
    ZUInt8 argCount = 0;
    if (!check(TOKEN_RIGHT_PAREN))
    {
        do
        {
            expression();
            if (argCount == MAX_ARGS)
            {
                char message[64];
                snprintf(message, sizeof message,
                         "Impossible d'avoir plus de %d arguments.", MAX_ARGS);
                error(message);
            }

            argCount++;
        } while (match(TOKEN_COMMA));
    }

    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après les arguments.");

    return argCount;
}

static ParseRule *getRule(TokenType type)
{
    return &rules[type];
}

static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}

static void block()
{
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF))
    {
        declaration();
    }

    consume(TOKEN_RIGHT_BRACE, "Un '}' est attendu après le bloc.");
}

static void function(FunctionType type)
{
    Compiler compiler;
    initCompiler(&compiler, type);
    beginScope();

    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après le nom de la fonction.");

    if (!check(TOKEN_RIGHT_PAREN))
    {
        do
        {
            current->function->arity++;
            if (current->function->arity > 255)
            {
                errorAtCurrent("Impossible d'avoir plus de %d paramètres. ", MAX_ARGS);
            }
            ZUInt8 constant = parseVariable(" Nom de paramètre attendu.");
            defineVariable(constant);
        } while (match(TOKEN_COMMA));
    }

    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après les paramètres.");
    consume(TOKEN_LEFT_BRACE, "Accolade '{' attendue avant le corps de la fonction.");
    block();

    ObjFunction *function = endCompiler();
    emitBytes(OP_CLOSURE, makeConstant(OBJ_VAL(function)));

    for (ZInt32 i = 0; i < function->upvalueCount; i++)
    {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0);
        emitByte(compiler.upvalues[i].index);
    }
}

static void funcDeclaration()
{
    ZUInt8 global = parseVariable("Expect function name.");
    markInitialized();
    function(TYPE_FUNCTION);
    defineVariable(global);
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
        // if the user does not initialize a variable zia compiler should initialize it to nul.
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
    // This now stores the scope depth
    beginLoop();

    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après boucle 'pour'.");

    // Initializer clause
    if (match(TOKEN_SEMICOLON))
    {
        // No initializer
    }
    else if (match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        expressionStatement();
    }

    // Set loop start position
    current->loopContext.loops[current->loopContext.loopDepth - 1].loopStart = currentChunk()->count;
    ZInt32 exitJump = -1;

    // Condition clause
    if (!match(TOKEN_SEMICOLON))
    {
        expression();
        consume(TOKEN_SEMICOLON, "Point-virgule ';' attendu après la condition.");

        exitJump = emitJump(OP_JUMP_IF_FALSE);
        // Pop condition if true
        emitByte(OP_POP);
    }

    // Increment clause
    ZInt32 incrementStart = -1;
    if (!match(TOKEN_RIGHT_PAREN))
    {
        // Jump over increment for first iteration
        ZInt32 bodyJump = emitJump(OP_JUMP);
        incrementStart = currentChunk()->count;

        // Store increment start for continue jumps
        current->loopContext.loops[current->loopContext.loopDepth - 1].incrementStart = incrementStart;

        expression();
        emitByte(OP_POP);
        consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après les clauses.");

        emitLoop(current->loopContext.loops[current->loopContext.loopDepth - 1].loopStart);
        patchJump(bodyJump);
    }

    // Set where breaks should jump to (after the loop)
    current->loopContext.loops[current->loopContext.loopDepth - 1].loopEnd = currentChunk()->count;

    // Loop body
    statement();

    // Jump back to condition/increment
    if (incrementStart != -1)
    {
        emitLoop(incrementStart);
    }
    else
    {
        emitLoop(current->loopContext.loops[current->loopContext.loopDepth - 1].loopStart);
    }

    // Patch exit jump if condition exists
    if (exitJump != -1)
    {
        patchJump(exitJump);
        emitByte(OP_POP);
    }

    endLoop();
    endScope();
}

static void breakStatement()
{
    consume(TOKEN_SEMICOLON, "Virgule ';' attendu après 'quitter'.");

    // Clear locals from scope
    while (current->localCount > 0 &&
           current->locals[current->localCount - 1].depth > current->scopeDepth)
    {
        emitByte(OP_POP);
        current->localCount--;
    }

    int jump = emitJump(OP_JUMP);

    if (current->switchContext.switchDepth > 0)
    {
        addBreakJump(jump);
    }
    else if (current->loopContext.loopDepth > 0)
    {
        addBreakJump(jump);
    }
    else
    {
        error("Les instructions 'quitter' ne peuvent être utilisées que dans une boucle ou un 'selon'.");
    }
}

static void continueStatement()
{
    if (current->loopContext.loopDepth == 0)
    {
        error("Les instructions 'continuer' ne peuvent être utilisées que dans une boucle.");
        return;
    }

    consume(TOKEN_SEMICOLON, "Un Virgule ';' est attendu après 'continuer'.");

    // Clean up locals in the loop scope
    Loop *currentLoop = &current->loopContext.loops[current->loopContext.loopDepth - 1];
    while (current->localCount > 0 &&
           current->locals[current->localCount - 1].depth > currentLoop->scopeDepth)
    {
        emitByte(OP_POP);
        current->localCount--;
    }

    // For while loops, emit a direct loop jump
    if (currentLoop->incrementStart != -1)
    {
        emitLoop(currentLoop->incrementStart);
    }
    else
    {
        emitLoop(currentLoop->loopStart);
    }
}

static void switchStatement()
{
    current->switchContext.switchDepth++;

    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après 'selon'.");
    expression(); // leaves discriminant on stack
    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après 'selon'.");
    consume(TOKEN_LEFT_BRACE, "Accolade '{' attendue avant le corps du 'selon'.");

    // store into temp local
    ZInt32 slot = current->localCount++;
    emitBytes(OP_SET_LOCAL, (ZUInt8)slot);

    // collect end‐of‐switch jumps
    ZInt32 endJumps[MAX_CASES];
    ZInt32 endCount = 0;

    // --- first case ---
    if (!match(TOKEN_CASE))
    {
        error("'selon' doit avoir au moins un 'cas'.");
        return;
    }
    advance();
    ZInt32 cv = atoi(parser.previous.start);
    consume(TOKEN_COLON, "':' attendu après 'cas N:'.");

    // if (slot != cv) skip this body
    emitBytes(OP_GET_LOCAL, (ZUInt8)slot);
    emitBytes(OP_CONSTANT, makeConstant(NUMBER_VAL(cv)));
    emitByte(OP_EQUAL);
    ZInt32 skip = emitJump(OP_JUMP_IF_FALSE);
    // pop the boolean
    emitByte(OP_POP);

    // body
    while (!check(TOKEN_CASE) && !check(TOKEN_DEFAULT) && !check(TOKEN_RIGHT_BRACE))
    {
        statement();
    }

    // jump to end
    endJumps[endCount++] = emitJump(OP_JUMP);
    patchJump(skip);
    // pop the boolean
    emitByte(OP_POP);

    // --- subsequent cases ---
    while (match(TOKEN_CASE))
    {
        advance();
        cv = atoi(parser.previous.start);
        consume(TOKEN_COLON, "':' attendu après 'cas N:'.");

        emitBytes(OP_GET_LOCAL, (ZUInt8)slot);
        emitBytes(OP_CONSTANT, makeConstant(NUMBER_VAL(cv)));
        emitByte(OP_EQUAL);
        skip = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP);

        while (!check(TOKEN_CASE) && !check(TOKEN_DEFAULT) && !check(TOKEN_RIGHT_BRACE))
        {
            statement();
        }

        endJumps[endCount++] = emitJump(OP_JUMP);
        patchJump(skip);
        emitByte(OP_POP);
    }

    // --- default ---
    if (match(TOKEN_DEFAULT))
    {
        consume(TOKEN_COLON, "':' attendu après 'défaut:'.");
        while (!check(TOKEN_RIGHT_BRACE))
        {
            statement();
        }
    }

    consume(TOKEN_RIGHT_BRACE, "Accolade '}' attendue a la fin du 'selon'.");

    // patch all end‐of‐switch jumps
    for (int i = 0; i < endCount; i++)
    {
        patchJump(endJumps[i]);
    }

    // patch all ‘quitter’ inside switch
    for (int i = 0; i < current->switchContext.switchBreakCount; i++)
    {
        patchJump(current->switchContext.switchBreakJumps[i]);
    }
    free(current->switchContext.switchBreakJumps);
    current->switchContext.switchBreakJumps = NULL;
    current->switchContext.switchBreakCount = 0;
    current->switchContext.switchDepth--;

    // pop temp local
    current->localCount--;
}

static void ifStatement()
{
    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après 'si'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après la condition.");

    ZInt32 thenJump = emitJump(OP_JUMP_IF_FALSE);
    // Pop condition if true
    emitByte(OP_POP);
    statement();

    ZInt32 elseJump = emitJump(OP_JUMP);
    patchJump(thenJump);
    // Pop condition if false
    emitByte(OP_POP);

    // Handle 'sinon si' and 'sinon'
    while (match(TOKEN_ELSE_IF))
    {
        consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après 'sinon si'.");
        expression();
        consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après la condition.");

        ZInt32 elseifJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP); // Pop condition if true
        statement();

        ZInt32 nextJump = emitJump(OP_JUMP);
        patchJump(elseifJump);
        emitByte(OP_POP); // Pop condition if false

        elseJump = nextJump;
    }

    if (match(TOKEN_ELSE))
    {
        statement();
    }

    patchJump(elseJump);
}

static void printStatement()
{
    do
    {
        expression();
        emitByte(OP_PRINT);
    } while (match(TOKEN_COMMA));

    consume(TOKEN_SEMICOLON, "Erreur : point-virgule manquant après la valeur.");
}

static void returnStatement()
{
    if (TYPE_SCRIPT == current->type)
    {
        error("Impossible de faire un 'returner' au niveau supérieur.");
    }

    if (match(TOKEN_SEMICOLON))
    {
        emitReturn();
    }
    else
    {
        expression();
        consume(TOKEN_SEMICOLON, "Point-virgule ';' attendu après la valeur de retour.");
        emitByte(OP_RETURN);
    }
}

static void whileStatement()
{
    beginScope();
    beginLoop();

    current->loopContext.loops[current->loopContext.loopDepth - 1].loopStart = currentChunk()->count;

    consume(TOKEN_LEFT_PAREN, "Parenthèse '(' attendue après boucle 'tantque'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Parenthèse ')' attendue après une condition.");

    // jump out if condition is false;
    ZInt32 exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);

    // Set where breaks should jump to (after the loop)
    current->loopContext.loops[current->loopContext.loopDepth - 1].loopEnd = currentChunk()->count;

    // Loop body
    statement();

    // Jump back to condition check
    emitLoop(current->loopContext.loops[current->loopContext.loopDepth - 1].loopStart);

    // Patch exit jump (points here when condition is false)
    patchJump(exitJump);
    emitByte(OP_POP);

    endLoop();
    endScope(); // End the while loop scope
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
        case TOKEN_BREAK:
        case TOKEN_CONTINUE:
        case TOKEN_SWITCH:
        case TOKEN_RETURN:
            return;
        default:;
        }

        advance();
    }
}

static void declaration()
{
    if (match(TOKEN_FUN))
    {
        funcDeclaration();
    }
    else if (match(TOKEN_VAR))
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
    else if (match(TOKEN_FOR))
    {
        forStatement();
    }
    else if (match(TOKEN_IF))
    {
        ifStatement();
    }
    else if (match(TOKEN_WHILE))
    {
        whileStatement();
    }
    else if (match(TOKEN_BREAK))
    {
        breakStatement();
    }
    else if (match(TOKEN_CONTINUE))
    {
        continueStatement();
    }
    else if (match(TOKEN_SWITCH))
    {
        switchStatement();
    }
    else if (match(TOKEN_LEFT_BRACE))
    {
        beginScope();
        block();
        endScope();
    }
    else if (match(TOKEN_RETURN))
    {
        returnStatement();
    }
    else
    {
        expressionStatement();
    }
}

ObjFunction *compile(const ZChar *source)
{
    initScanner(source);
    Compiler compiler;
    initCompiler(&compiler, TYPE_SCRIPT);

    parser.hadError = false;
    parser.panicMode = false;

    advance();

    while (!match(TOKEN_EOF))
    {
        declaration();
    }

    ObjFunction *function = endCompiler();
    /*
    @NOTE: This way, the VM doesn’t try to execute a function that may contain invalid bytecode.
    */
    return parser.hadError ? NULL : function;
}

void markCompilerRoots()
{
    Compiler *compiler = current;
    while (NULL != compiler)
    {
        markObject((Obj *)compiler->function);
        compiler = compiler->enclosing;
    }
}
