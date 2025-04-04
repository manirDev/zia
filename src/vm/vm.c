#include "vm.h"
#include <stdio.h>
#include "debug.h"

VM vm;
static void resetStack()
{
    vm.stackTop = vm.stack;
}
void initVM()
{
    resetStack();
}

void freeVM()
{

}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("      ");
        for(Value* slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (ZInt32)(vm.ip - vm.chunk->code));
#endif
       ZUInt8 instruction;
       switch (instruction = READ_BYTE())
       {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
       case OP_RETURN: {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
       }
       
       default:
        break;
       }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk* chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}