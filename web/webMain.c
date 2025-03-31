/**
 * main-web.c - WebAssembly entry point for the zia interpreter
 * 
 * This file handles the integration between the C interpreter and
 * the web browser environment through Emscripten.
 */

 #include <emscripten.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 // Define maximum buffer sizes
 #define MAX_SOURCE_SIZE 1024 * 1024  // 1MB max source code
 #define MAX_OUTPUT_SIZE 1024 * 1024  // 1MB max output
 
 // Buffer to store tokenization/interpretation results
 static char output_buffer[MAX_OUTPUT_SIZE];
 
 // External functions from the interpreter
 extern void* init_interpreter();
 extern char* tokenize(void* interpreter, const char* source);
 extern char* interpret(void* interpreter, const char* source);
 extern void free_interpreter(void* interpreter);
 
 // Global interpreter instance
 static void* interpreter = NULL;
 
 // Initialize the interpreter
 EMSCRIPTEN_KEEPALIVE
 void init() {
     if (interpreter == NULL) {
         interpreter = init_interpreter();
     }
 }
 
 // Tokenize source code
 EMSCRIPTEN_KEEPALIVE
 const char* web_tokenize(const char* source) {
     // Initialize if needed
     if (interpreter == NULL) {
         init();
     }
     
     // Clear the output buffer
     memset(output_buffer, 0, MAX_OUTPUT_SIZE);
     
     // Tokenize and store the result
     char* result = tokenize(interpreter, source);
     if (result) {
         strncpy(output_buffer, result, MAX_OUTPUT_SIZE - 1);
         free(result);
     } else {
         strcpy(output_buffer, "Error: Tokenization failed");
     }
     
     return output_buffer;
 }
 
 // Interpret source code
 EMSCRIPTEN_KEEPALIVE
 const char* web_interpret(const char* source) {
     // Initialize if needed
     if (interpreter == NULL) {
         init();
     }
     
     // Clear the output buffer
     memset(output_buffer, 0, MAX_OUTPUT_SIZE);
     
     // Interpret and store the result
     char* result = interpret(interpreter, source);
     if (result) {
         strncpy(output_buffer, result, MAX_OUTPUT_SIZE - 1);
         free(result);
     } else {
         strcpy(output_buffer, "Error: Interpretation failed");
     }
     
     return output_buffer;
 }
 
 // Clean up resources
 EMSCRIPTEN_KEEPALIVE
 void cleanup() {
     if (interpreter != NULL) {
         free_interpreter(interpreter);
         interpreter = NULL;
     }
 }
 
 // Print to console (for debugging)
 EMSCRIPTEN_KEEPALIVE
 void console_log(const char* message) {
     printf("%s\n", message);
 }
 
 // Main function (required by Emscripten)
 int main() {
     init();
     return 0;
 }