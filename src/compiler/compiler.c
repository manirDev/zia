#include "compiler.h"
#include "common/common.h"
#include "scanner/scanner.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    Token current;
    Token previous;
    ZBool hadError;
    ZBool panicMode;
}Parser;

Parser parser;

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
        if (parser.current.type != TOKEN_EOF)
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

ZBool compile(const char* source, Chunk* chunk)
{
    initScanner(source);
    parser.hadError = false;
    parser.panicMode = false;
    advance();
    expression();
    consume(TOKEN_EOF, "Fin d'expression attendue.");
    return (!parser.hadError);
}