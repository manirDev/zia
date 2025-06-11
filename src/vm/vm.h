#ifndef ZAI_VM_H
#define ZAI_VM_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "chunk/chunk.h"
#include "table/table.h"
#include "object/object.h"

#define FRAMES_MAX  64
#define STACK_MAX   (FRAMES_MAX * UINT8_COUNT)

typedef struct
{
    ObjClosure* closure;
    ZUInt8* ip;
    Value* slots;
}CallFrame;

typedef struct
{
   CallFrame frames[FRAMES_MAX];
   ZInt32 frameCount;
   Value stack[STACK_MAX];
   Value* stackTop;
   Table strings;
   Table globals;
   ObjUpvalue* openUpvalues;
   size_t bytesAllocated;
   size_t nextGC;
   Obj* objects;
   ZInt32 grayCount;
   ZInt32 grayCapacity;
   Obj** grayStack;
}VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
}InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const ZChar* source);
void push(Value value);
Value pop();

#endif