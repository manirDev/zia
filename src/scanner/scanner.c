#include "scanner.h"
#include "common/common.h"
#include "common/commonTypes.h"
#include <stdio.h>
#include <string.h>

#define END_CHAR '\0'

typedef struct
{
    const char* start;
    const char* current;
    ZInt32 line;
}Scanner;

Scanner scanner;

static ZBool isAtEnd()
{
    return *scanner.current == END_CHAR;
}

static Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (ZInt32)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (ZInt32)strlen(message);
    token.line = scanner.line;

    return token;
}

void initScanner(const char* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scanToken()
{
    scanner.start = scanner.current;
    if (isAtEnd())
    {
        return makeToken(TOKEN_EOF);
    }
    
    return errorToken("Caractère inattendu.");
}