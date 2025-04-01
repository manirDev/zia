#ifndef ZAI_VM_H
#define ZAI_VM_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "chunk/chunk.h"

#define STACK_MAX 256

typedef struct
{
   Chunk* chunk;
   ZUInt8* ip;
   Value stack[STACK_MAX];
   Value* stackTop;
}VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNUTIME_ERROR,
}InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();

#endif