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

static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

static ZBool match(char expected)
{
    if (isAtEnd())
    {
        return ZFALSE;
    }
    if (*scanner.current != expected)
    {
        return ZFALSE;
    }
    scanner.current++;
    return true;
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

    char currChar = advance();
    switch (currChar)
    {
    case '(':
    {
        return makeToken(TOKEN_LEFT_PAREN);
    }
    case ')':
    {
        return makeToken(TOKEN_RIGHT_PAREN);
    }
    case '{':
    {
        return makeToken(TOKEN_LEFT_BRACE);
    }
    case '}':
    {
        return makeToken(TOKEN_RIGHT_BRACE);
    }
    case ';':
    {
        return makeToken(TOKEN_SEMICOLON);
    }
    case ',':
    {
        return makeToken(TOKEN_COMMA);
    }
    case '.':
    {
        return makeToken(TOKEN_DOT);
    }
    case '-':
    {
        return makeToken(TOKEN_MINUS);
    }
    case '+':
    {
        return makeToken(TOKEN_PLUS);
    }
    case '/':
    {
        return makeToken(TOKEN_SLASH);
    }
    case '*':
    {
        return makeToken(TOKEN_STAR);
    }
    case '!':
    {
        return makeToken(
            match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG
        );
    }
    case '=':
    {
        return makeToken(
            match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL
        );
    }
    case '<':
    {
        return makeToken(
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS
        );
    }
    case '>':
    {
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER
        );
    }
    default:
        break;
    }
    
    return errorToken("Caractère inattendu.");
}