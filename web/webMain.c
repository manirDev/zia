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

#include "../src/common/common.h"
#include "../src/chunk/chunk.h"
#include "../debug/debug.h"
#include "../src/vm/vm.h"

#define COMPILE_TIME_EXIT_CODE  65
#define RUN_TIME_EXIT_CODE      70

EMSCRIPTEN_KEEPALIVE
ZInt32 runCompiler(ZChar* sourceCode, ZBool activateBC, ZBool activateTE, ZBool activateGC)
{
#ifdef DEBUG_PRINT_CODE
    FLAG_PRINT_CODE = activateBC;
    if(ZTRUE == FLAG_PRINT_CODE)
    {
        printf("FLAG_PRINT_BYTECODE is ON\n");
    }
#endif

#ifdef DEBUG_TRACE_EXECUTION
    FLAG_TRACE_EXECUTION = activateTE;
    if (ZTRUE == FLAG_TRACE_EXECUTION)
    {
       printf("FLAG_TRACE_EXECUTION is ON\n");
    }
    
#endif

#ifdef DEBUG_LOG_GC
    FLAG_LOG_GC = activateGC;
    if (ZTRUE == FLAG_LOG_GC)
    {
       printf("FLAG_LOG_GC is ON\n");
    }
#endif

    printf("========== Code En Compilation ==========\n\n");

    initVM();
    InterpretResult result = interpret(sourceCode);
    printf("\n");
    free(sourceCode);
    freeVM();

    if (INTERPRET_COMPILE_ERROR == result)
    {
        printf("Arrêté avec: INTERPRET_COMPILE_ERROR!\n");
        return COMPILE_TIME_EXIT_CODE;
    }
    if (INTERPRET_RUNTIME_ERROR == result)
    {
        printf("Arrêté avec: INTERPRET_RUNTIME_ERROR!\n");
        return RUN_TIME_EXIT_CODE;
    }
    return 0;
}
 