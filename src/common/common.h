#ifndef ZIA_COMMON_H
#define ZIA_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BLANK_SPACE ' '
#define NEW_LINE '\n'
#define TAB_SPACE '\t'
#define CARRIAGE_RETURN '\r'
#define COMMENT_SLASH '/'
#define COMMENT_STAR '*'
#define NULL_CHAR '\0'

#define DEBUG_PRINT_CODE            // FLAG to enable Dumping generated Chunks (of bytecode)
#define DEBUG_TRACE_EXECUTION       // FLAG to enable Diagnostics/Debug print outs

#define DEBUG_STRESS_GC             // FLAG triggers GC EVERY time it can. Used to find GC-Bugs, that only happen when GC-triggers etc.
#define DEBUG_LOG_GC                // FLAG to enable Diagnostics print outs for Garbage Collection

#define UINT8_COUNT (UINT8_MAX + 1) // Hard limit on how many locals can exist at the same time (IN THE SAME SCOPE)

//#undef DEBUG_PRINT_CODE             // comment this out: to enable debug printing
//#undef DEBUG_TRACE_EXECUTION        // comment this out: to enable trace-execution

#undef DEBUG_STRESS_GC              // comment this out: to enable GC every step
///#undef DEBUG_LOG_GC                 // comment this out: to enable loging of GC steps

// flag-variables, set in main-implementations, to toggle on/off GC. (ex. in Wasm-Web-Frontend)
#ifdef DEBUG_PRINT_CODE
extern bool FLAG_PRINT_CODE;
#endif

#ifdef DEBUG_TRACE_EXECUTION
extern bool FLAG_TRACE_EXECUTION;
#endif

#ifdef DEBUG_LOG_GC
extern bool FLAG_LOG_GC;
#endif

#endif