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

static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static ZBool isAtEnd()
{
    return *scanner.current == END_CHAR;
}

static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

static char peek()
{
    return *scanner.current;
}

static char peekNext()
{
    if (isAtEnd())
    {
        return NULL_CHAR;
    }
    return scanner.current[1];    
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

static void skipWhiteSpace()
{
    for(;;)
    {
        char c = peek();
        switch (c)
        {
        case BLANK_SPACE:
        case CARRIAGE_RETURN:
        case TAB_SPACE:
            advance();
            break;
        case NEW_LINE:
            scanner.line++;
            advance();
            break;
        case COMMENT_SLASH:
            if(peekNext() == COMMENT_SLASH)
            {
                while(peek() != NEW_LINE && !isAtEnd()) 
                {
                    advance();
                }
            }
            else
            {
                return;
            }
            break;
        default:
            return;
        }
    }
}

static TokenType checkKeyword(ZInt32 start, ZInt32 length, const ZChar* rest, TokenType type)
{
    if ((scanner.current - scanner.start == start + length) && (memcmp(scanner.start + start, rest, length) == 0))
    {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType()
{
    ZInt32 length = scanner.current - scanner.start;
    switch (scanner.start[0])
    {
        case 'e': return checkKeyword(1, 1, "t", TOKEN_AND);
        case 'p': return checkKeyword(1, 3, "our", TOKEN_FOR);
        case 'n': return checkKeyword(1, 2, "ul", TOKEN_NULL);
        case 'o': return checkKeyword(1, 1, "u", TOKEN_OR);
        case 'a': return checkKeyword(1, 7, "fficher", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 8, "etourner", TOKEN_RETURN);
        case 't': return checkKeyword(1, 6, "antque", TOKEN_WHILE);
        case 'c': 
        {
            if (length > 1)
            {
                switch (scanner.start[1])
                {
                    case 'l': return checkKeyword(2, 4, "asse", TOKEN_CLASS);
                    case 'e': return checkKeyword(2, 2, "ci", TOKEN_THIS);
                }
            }
            break;
        }
        
        case 'v':
        {
            if (length > 1)
            {
                switch (scanner.start[1])
                {
                    case 'r': return checkKeyword(2, 2, "ai", TOKEN_TRUE);
                    case 'a': return checkKeyword(2, 1, "r", TOKEN_VAR);
                }
            }
            break;
        }
        case 'f':
        {
            if (length > 1)
            {
                switch (scanner.start[1])
                {
                    case 'o': return checkKeyword(2, 1, "n", TOKEN_FUN);
                    case 'a': return checkKeyword(2, 2, "ux", TOKEN_FALSE);
                }
            }
            break;
        }
        case 's':
        {
            if (scanner.current - scanner.start > 1)
            {
                switch (scanner.start[1])
                {
                    case 'u': return checkKeyword(2, 3, "per", TOKEN_SUPER);
                    case 'i':
                    {
                        if (length == 2)
                        {
                            return TOKEN_IF;
                        }
                        else
                        {
                            return checkKeyword(2, 3, "non", TOKEN_ELSE);
                        }
                    }
                }
            }
            break;
        }

        default:
            break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
    {
        advance();
    }
    return makeToken(identifierType());
}

static Token number()
{
    while(isDigit(peek()))
    {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext( )))
    {
        advance();
        while(isDigit(peek()))
        {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

static Token string()
{
    while(peek() != '"' && !isAtEnd())
    {
        if (peek() == NEW_LINE)
        {
            scanner.line++;
        }
        advance();
    }
    if (isAtEnd())
    {
        return errorToken("Chaîne non terminée.");
    }
    advance();
    return makeToken(TOKEN_STRING);
}

void initScanner(const ZChar* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token scanToken()
{
    skipWhiteSpace();
    scanner.start = scanner.current;
    if (isAtEnd())
    {
        return makeToken(TOKEN_EOF);
    }

    ZChar currChar = advance();
    if (isAlpha(currChar))
    {
        return identifier();
    }
    
    if (isDigit(currChar))
    {
        return number();
    }
    
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
    case '"':
    {
        return string();
    }
    case ':':
    {
        return makeToken(TOKEN_COLON);
    }
    case '?':
    {
        return makeToken(TOKEN_QUESTION);
    }
    default:
        break;
    }
    
    return errorToken("Caractère inattendu.");
}