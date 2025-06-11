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

// we define needed Flags:
#ifdef DEBUG_PRINT_CODE
ZBool FLAG_PRINT_CODE = false;
#endif

#ifdef DEBUG_TRACE_EXECUTION
ZBool FLAG_TRACE_EXECUTION = false;
#endif

#ifdef DEBUG_LOG_GC
ZBool FLAG_LOG_GC = false;
#endif


EMSCRIPTEN_KEEPALIVE
ZInt32 runCompiler(ZChar* sourceCode)
{
#ifdef DEBUG_PRINT_CODE
    if(ZTRUE == FLAG_PRINT_CODE)
    {
        printf("FLAG_PRINT_BYTECODE is ON\n");
    }
#endif

#ifdef DEBUG_TRACE_EXECUTION
    if (ZTRUE == FLAG_TRACE_EXECUTION)
    {
       printf("FLAG_TRACE_EXECUTION is ON\n");
    }
    
#endif

#ifdef DEBUG_LOG_GC
    if (ZTRUE == FLAG_LOG_GC)
    {
       printf("FLAG_LOG_GC is ON\n");
    }
    
#endif

    printf("========== Compiling Code ==========\n\n");

    initVM();
    InterpretResult result = interpret(sourceCode);
    freeVM();

    printf("\n");

    if (INTERPRET_COMPILE_ERROR == result)
    {
        printf("Stopped with: INTERPRET_COMPILE_ERROR!\n");
        return COMPILE_TIME_EXIT_CODE;
    }
    if (INTERPRET_RUNTIME_ERROR == result)
    {
        printf("Stopped with: INTERPRET_RUNTIME_ERROR!\n");
        return RUN_TIME_EXIT_CODE;
    }
    return 0;
}
 