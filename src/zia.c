/**
 * zia.c - A small interpreter with French keywords
 * 
 * This implements a simple interpreter for a JavaScript-like language
 * with French keywords.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 
 // Token types
 typedef enum {
     // Keywords
     TOKEN_VAR,       // variable (variable)
     TOKEN_FONCTION,  // fonction (function)
     TOKEN_SI,        // si (if)
     TOKEN_SINON,     // sinon (else)
     TOKEN_POUR,      // pour (for)
     TOKEN_TANTQUE,   // tantque (while)
     TOKEN_RETOURNE,  // retourne (return)
     TOKEN_AFFICHE,   // affiche (print)
     TOKEN_VRAI,      // vrai (true)
     TOKEN_FAUX,      // faux (false)
     TOKEN_NUL,       // nul (null)
     
     // Literals
     TOKEN_IDENTIFIER,// identifiers
     TOKEN_STRING,    // string literal
     TOKEN_NUMBER,    // numeric literal
     
     // Operators and punctuation
     TOKEN_LEFT_PAREN,// (
     TOKEN_RIGHT_PAREN,// )
     TOKEN_LEFT_BRACE,// {
     TOKEN_RIGHT_BRACE,// }
     TOKEN_COMMA,     // ,
     TOKEN_DOT,       // .
     TOKEN_MINUS,     // -
     TOKEN_PLUS,      // +
     TOKEN_SEMICOLON, // ;
     TOKEN_SLASH,     // /
     TOKEN_STAR,      // *
     TOKEN_BANG,      // !
     TOKEN_BANG_EQUAL,// !=
     TOKEN_EQUAL,     // =
     TOKEN_EQUAL_EQUAL,// ==
     TOKEN_GREATER,   // >
     TOKEN_GREATER_EQUAL,// >=
     TOKEN_LESS,      // <
     TOKEN_LESS_EQUAL,// <=
     
     // Special tokens
     TOKEN_ERROR,     // error token
     TOKEN_EOF        // end of file
 } TokenType;
 
 // Token structure
 typedef struct {
     TokenType type;
     const char* start;
     int length;
     int line;
     const char* lexeme;  // Copy of the token text
 } Token;
 
 // Scanner structure
 typedef struct {
     const char* start;
     const char* current;
     int line;
 } Scanner;
 
 // Interpreter structure
 typedef struct {
     Scanner scanner;
     Token current;
     Token previous;
     bool hadError;
     bool panicMode;
 } Interpreter;
 
 // Forward declarations
 static Token scanToken(Scanner* scanner);
 static Token errorToken(Scanner* scanner, const char* message);
 
 // Initialize the interpreter
 void* init_interpreter() {
     Interpreter* interpreter = (Interpreter*)malloc(sizeof(Interpreter));
     if (interpreter == NULL) return NULL;
     
     memset(interpreter, 0, sizeof(Interpreter));
     //interpreter->line = 1;
     
     return interpreter;
 }
 
 // Free the interpreter
 void free_interpreter(void* interpreter) {
     if (interpreter != NULL) {
         free(interpreter);
     }
 }
 
 // Initialize the scanner
 static void initScanner(Scanner* scanner, const char* source) {
     scanner->start = source;
     scanner->current = source;
     scanner->line = 1;
 }
 
 // Check if character is a digit
 static bool isDigit(char c) {
     return c >= '0' && c <= '9';
 }
 
 // Check if character is a letter or underscore
 static bool isAlpha(char c) {
     return (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
 }
 
 // Check if character is alphanumeric
 static bool isAlphaNumeric(char c) {
     return isAlpha(c) || isDigit(c);
 }
 
 // Check if we've reached the end of the source
 static bool isAtEnd(Scanner* scanner) {
     return *scanner->current == '\0';
 }
 
 // Advance the scanner one character
 static char advance(Scanner* scanner) {
     return *scanner->current++;
 }
 
 // Peek at the current character without advancing
 static char peek(Scanner* scanner) {
     return *scanner->current;
 }
 
 // Look at the next character without advancing
 static char peekNext(Scanner* scanner) {
     if (isAtEnd(scanner)) return '\0';
     return scanner->current[1];
 }
 
 // Match the current character and advance if it matches
 static bool match(Scanner* scanner, char expected) {
     if (isAtEnd(scanner)) return false;
     if (*scanner->current != expected) return false;
     scanner->current++;
     return true;
 }
 
 // Create a token
 static Token makeToken(Scanner* scanner, TokenType type) {
     Token token;
     token.type = type;
     token.start = scanner->start;
     token.length = (int)(scanner->current - scanner->start);
     token.line = scanner->line;
     
     // Make a copy of the lexeme
     char* lexeme = (char*)malloc(token.length + 1);
     if (lexeme != NULL) {
         memcpy(lexeme, token.start, token.length);
         lexeme[token.length] = '\0';
         token.lexeme = lexeme;
     } else {
         token.lexeme = NULL;
     }
     
     return token;
 }
 
 // Create an error token
 static Token errorToken(Scanner* scanner, const char* message) {
     Token token;
     token.type = TOKEN_ERROR;
     token.start = message;
     token.length = (int)strlen(message);
     token.line = scanner->line;
     token.lexeme = strdup(message);
     return token;
 }
 
 // Skip whitespace and comments
 static void skipWhitespace(Scanner* scanner) {
     for (;;) {
         char c = peek(scanner);
         switch (c) {
             case ' ':
             case '\r':
             case '\t':
                 advance(scanner);
                 break;
             case '\n':
                 scanner->line++;
                 advance(scanner);
                 break;
             case '/':
                 if (peekNext(scanner) == '/') {
                     // A comment goes until the end of the line
                     while (peek(scanner) != '\n' && !isAtEnd(scanner)) {
                         advance(scanner);
                     }
                 } else {
                     return;
                 }
                 break;
             default:
                 return;
         }
     }
 }
 
 // Check if the current lexeme matches a keyword
 static TokenType checkKeyword(Scanner* scanner, int start, int length, 
                              const char* rest, TokenType type) {
     if (scanner->current - scanner->start == start + length &&
         memcmp(scanner->start + start, rest, length) == 0) {
         return type;
     }
     
     return TOKEN_IDENTIFIER;
 }
 
 // Determine the type of an identifier
 static TokenType identifierType(Scanner* scanner) {
     switch (scanner->start[0]) {
         case 'a': 
             if (scanner->current - scanner->start > 1) {
                 if (scanner->start[1] == 'f')
                     return checkKeyword(scanner, 2, 5, "fiche", TOKEN_AFFICHE);
             }
             break;
         case 'f': 
             if (scanner->current - scanner->start > 1) {
                 if (scanner->start[1] == 'a')
                     return checkKeyword(scanner, 2, 2, "ux", TOKEN_FAUX);
                 if (scanner->start[1] == 'o')
                     return checkKeyword(scanner, 2, 6, "nction", TOKEN_FONCTION);
             }
             break;
         case 'n': return checkKeyword(scanner, 1, 2, "ul", TOKEN_NUL);
         case 'p': return checkKeyword(scanner, 1, 3, "our", TOKEN_POUR);
         case 'r': return checkKeyword(scanner, 1, 7, "etourne", TOKEN_RETOURNE);
         case 's': 
             if (scanner->current - scanner->start > 1) {
                 if (scanner->start[1] == 'i') {
                     if (scanner->current - scanner->start == 2)
                         return TOKEN_SI;
                     return checkKeyword(scanner, 2, 3, "non", TOKEN_SINON);
                 }
             }
             break;
         case 't': return checkKeyword(scanner, 1, 6, "antque", TOKEN_TANTQUE);
         case 'v': 
             if (scanner->current - scanner->start > 1) {
                 if (scanner->start[1] == 'a')
                     return checkKeyword(scanner, 2, 1, "r", TOKEN_VAR);
                 if (scanner->start[1] == 'r')
                     return checkKeyword(scanner, 2, 2, "ai", TOKEN_VRAI);
             }
             break;
     }
     
     return TOKEN_IDENTIFIER;
 }
 
 // Scan an identifier
 static Token identifier(Scanner* scanner) {
     while (isAlphaNumeric(peek(scanner))) advance(scanner);
     
     return makeToken(scanner, identifierType(scanner));
 }
 
 // Scan a number
 static Token number(Scanner* scanner) {
     while (isDigit(peek(scanner))) advance(scanner);
     
     // Look for a decimal point
     if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
         // Consume the "."
         advance(scanner);
         
         while (isDigit(peek(scanner))) advance(scanner);
     }
     
     return makeToken(scanner, TOKEN_NUMBER);
 }
 
 // Scan a string
 static Token string(Scanner* scanner) {
     while (peek(scanner) != '"' && !isAtEnd(scanner)) {
         if (peek(scanner) == '\n') scanner->line++;
         advance(scanner);
     }
     
     if (isAtEnd(scanner)) return errorToken(scanner, "Chaîne non terminée.");
     
     // The closing quote
     advance(scanner);
     return makeToken(scanner, TOKEN_STRING);
 }
 
 // Scan the next token
 static Token scanToken(Scanner* scanner) {
     skipWhitespace(scanner);
     
     scanner->start = scanner->current;
     
     if (isAtEnd(scanner)) return makeToken(scanner, TOKEN_EOF);
     
     char c = advance(scanner);
     
     if (isAlpha(c)) return identifier(scanner);
     if (isDigit(c)) return number(scanner);
     
     switch (c) {
         case '(': return makeToken(scanner, TOKEN_LEFT_PAREN);
         case ')': return makeToken(scanner, TOKEN_RIGHT_PAREN);
         case '{': return makeToken(scanner, TOKEN_LEFT_BRACE);
         case '}': return makeToken(scanner, TOKEN_RIGHT_BRACE);
         case ';': return makeToken(scanner, TOKEN_SEMICOLON);
         case ',': return makeToken(scanner, TOKEN_COMMA);
         case '.': return makeToken(scanner, TOKEN_DOT);
         case '-': return makeToken(scanner, TOKEN_MINUS);
         case '+': return makeToken(scanner, TOKEN_PLUS);
         case '/': return makeToken(scanner, TOKEN_SLASH);
         case '*': return makeToken(scanner, TOKEN_STAR);
         case '!': 
             return makeToken(scanner, 
                 match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
         case '=': 
             return makeToken(scanner, 
                 match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
         case '<': 
             return makeToken(scanner, 
                 match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
         case '>': 
             return makeToken(scanner, 
                 match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
         case '"': return string(scanner);
     }
     
     return errorToken(scanner, "Caractère inattendu.");
 }
 
 // Tokenize source code
 char* tokenize(void* interpreter_ptr, const char* source) {
     if (source == NULL) return NULL;
     
     Interpreter* interpreter = (Interpreter*)interpreter_ptr;
     Scanner scanner;
     initScanner(&scanner, source);
     
     // Buffer for the output
     char* output = (char*)malloc(1024 * 1024); // 1MB buffer
     if (output == NULL) return NULL;
     output[0] = '\0';
     
     int pos = 0;
     Token token;
     
     do {
         token = scanToken(&scanner);
         
         const char* typeStr;
         switch (token.type) {
             case TOKEN_VAR: typeStr = "VAR"; break;
             case TOKEN_FONCTION: typeStr = "FONCTION"; break;
             case TOKEN_SI: typeStr = "SI"; break;
             case TOKEN_SINON: typeStr = "SINON"; break;
             case TOKEN_POUR: typeStr = "POUR"; break;
             case TOKEN_TANTQUE: typeStr = "TANTQUE"; break;
             case TOKEN_RETOURNE: typeStr = "RETOURNE"; break;
             case TOKEN_AFFICHE: typeStr = "AFFICHE"; break;
             case TOKEN_VRAI: typeStr = "VRAI"; break;
             case TOKEN_FAUX: typeStr = "FAUX"; break;
             case TOKEN_NUL: typeStr = "NUL"; break;
             case TOKEN_IDENTIFIER: typeStr = "IDENTIFIANT"; break;
             case TOKEN_STRING: typeStr = "CHAINE"; break;
             case TOKEN_NUMBER: typeStr = "NOMBRE"; break;
             case TOKEN_LEFT_PAREN: typeStr = "PAREN_GAUCHE"; break;
             case TOKEN_RIGHT_PAREN: typeStr = "PAREN_DROITE"; break;
             case TOKEN_LEFT_BRACE: typeStr = "ACCOLADE_GAUCHE"; break;
             case TOKEN_RIGHT_BRACE: typeStr = "ACCOLADE_DROITE"; break;
             case TOKEN_COMMA: typeStr = "VIRGULE"; break;
             case TOKEN_DOT: typeStr = "POINT"; break;
             case TOKEN_MINUS: typeStr = "MOINS"; break;
             case TOKEN_PLUS: typeStr = "PLUS"; break;
             case TOKEN_SEMICOLON: typeStr = "POINT_VIRGULE"; break;
             case TOKEN_SLASH: typeStr = "SLASH"; break;
             case TOKEN_STAR: typeStr = "ETOILE"; break;
             case TOKEN_BANG: typeStr = "EXCLAMATION"; break;
             case TOKEN_BANG_EQUAL: typeStr = "NON_EGAL"; break;
             case TOKEN_EQUAL: typeStr = "EGAL"; break;
             case TOKEN_EQUAL_EQUAL: typeStr = "EGAL_EGAL"; break;
             case TOKEN_GREATER: typeStr = "SUPERIEUR"; break;
             case TOKEN_GREATER_EQUAL: typeStr = "SUPERIEUR_EGAL"; break;
             case TOKEN_LESS: typeStr = "INFERIEUR"; break;
             case TOKEN_LESS_EQUAL: typeStr = "INFERIEUR_EGAL"; break;
             case TOKEN_ERROR: typeStr = "ERREUR"; break;
             case TOKEN_EOF: typeStr = "FIN"; break;
             default: typeStr = "INCONNU"; break;
         }
         
         // Format token information as JSON
         char tokenInfo[1024];
         if (token.type == TOKEN_ERROR) {
             sprintf(tokenInfo, "{\"type\":\"%s\",\"message\":\"%s\",\"line\":%d}\n", 
                     typeStr, token.lexeme, token.line);
         } else if (token.type == TOKEN_EOF) {
             sprintf(tokenInfo, "{\"type\":\"%s\",\"line\":%d}\n", 
                     typeStr, token.line);
         } else {
             sprintf(tokenInfo, "{\"type\":\"%s\",\"lexeme\":\"%s\",\"line\":%d}\n", 
                     typeStr, token.lexeme, token.line);
         }
         
         // Append to output buffer
         int len = strlen(tokenInfo);
         if (pos + len < 1024 * 1024 - 1) {
             strcat(output + pos, tokenInfo);
             pos += len;
         }
         
         // Free the lexeme memory
         if (token.lexeme != NULL) {
             free((void*)token.lexeme);
         }
         
     } while (token.type != TOKEN_EOF);
     
     return output;
 }
 
 // Interpret source code (currently just tokenizes)
 char* interpret(void* interpreter_ptr, const char* source) {
     // For now, interpretation is just tokenization
     // This will be expanded in future versions
     return tokenize(interpreter_ptr, source);
 }
 
 #ifndef __EMSCRIPTEN__
 // Main function for native builds
 int main(int argc, char* argv[]) {
     if (argc < 2) {
         printf("Usage: %s <source_file>\n", argv[0]);
         return 1;
     }
     
     // Read the source file
     FILE* file = fopen(argv[1], "r");
     if (file == NULL) {
         printf("Impossible d'ouvrir le fichier '%s'\n", argv[1]);
         return 1;
     }
     
     fseek(file, 0L, SEEK_END);
     size_t fileSize = ftell(file);
     rewind(file);
     
     char* source = (char*)malloc(fileSize + 1);
     if (source == NULL) {
         printf("Pas assez de mémoire pour lire le fichier '%s'\n", argv[1]);
         fclose(file);
         return 1;
     }
     
     size_t bytesRead = fread(source, sizeof(char), fileSize, file);
     source[bytesRead] = '\0';
     fclose(file);
     
     // Initialize the interpreter
     void* interpreter = init_interpreter();
     
     // Tokenize the source
     char* tokens = tokenize(interpreter, source);
     printf("%s\n", tokens);
     free(tokens);
     
     // Clean up
     free_interpreter(interpreter);
     free(source);
     
     return 0;
 }
 #endif