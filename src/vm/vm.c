#include "vm.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"
#include "object/object.h"
#include "memory/memory.h"
#include "compiler/compiler.h"

VM vm;

static Value peek(ZInt32 distance);
static ZBool isFalsey(Value value);
static void concatenate();

static void resetStack()
{
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
}

static void runtimeError(const ZChar* format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    CallFrame* frame = &vm.frames[vm.frameCount - 1];
    size_t instruction = frame->ip -frame->function->chunk.code - 1;
    ZInt32 line = frame->function->chunk.lines[instruction];
    fprintf(stderr, "[ligne %d] dans le script.\n", line);
    resetStack();
}

static ZReal64 ziaFmod(ZReal64 a, ZReal64 b)
{
    ZReal64 quotient = (a / b);
    ZInt32 truncated  = (ZInt32)quotient;
    return (a - (b * truncated));
}

void initVM()
{
    resetStack();
    vm.objects = NULL;
    initTable(&vm.globals);
    initTable(&vm.strings);
}

void freeVM()
{
    freeTable(&vm.globals);
    freeTable(&vm.strings);
    freeObjects();
}

static InterpretResult run()
{
    CallFrame* frame = &vm.frames[vm.frameCount - 1];

#define READ_BYTE() (*frame->ip++)
#define READ_CONSTANT() (frame->function->chunk.constants.values[READ_BYTE()])
#define READ_SHORT() \
        (frame->ip += 2, (ZUInt16)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_24BIT_OFFSET() \
    (frame->ip += 3, \
    (ZUInt32)((frame->ip[-3] << 16) | \
              (frame->ip[-2] << 8) | \
               frame->ip[-1]))
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op)\
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) \
        { \
           runtimeError("Les opérandes doivent être des nombres."); \
           return INTERPRET_RUNTIME_ERROR; \
        } \
        ZReal64 b = AS_NUMBER(pop()); \
        ZReal64 a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
    } while(ZFALSE)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("      ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(&frame->function->chunk, (ZInt32)(frame->ip - frame->function->chunk.code));
#endif
        ZUInt8 instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_NULL:
        {
            push(NUL_VAL);
            break;
        }
        case OP_TRUE:
        {
            push(BOOL_VAL(true));
            break;
        }
        case OP_FALSE:
        {
            push(BOOL_VAL(false));
            break;
        }
        case OP_POP:
        {
            pop();
            break;
        }
        case OP_GET_LOCAL:
        {
            ZUInt8 slot = READ_BYTE();
            push(frame->slots[slot]);
            break;
        }
        case OP_SET_LOCAL:
        {
            ZUInt8 slot = READ_BYTE();
            frame->slots[slot] = peek(0);
            break;
        }
        case OP_SET_GLOBAL:
        {
            ObjString* name = READ_STRING();
            if (tableSet(&vm.globals, name, peek(0)))
            {
                tableDelete(&vm.globals, name);
                runtimeError("Variable '%s' non définie.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_GET_GLOBAL:
        {
            ObjString* name = READ_STRING();
            Value value;
            if (!tableGet(&vm.globals, name, &value))
            {
                runtimeError("Variable '%s' non définie.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            push(value);
            break;
            
        }
        case OP_DEFINE_GLOBAL:
        {
            ObjString* name = READ_STRING();
            tableSet(&vm.globals, name, peek(0));
            pop();
            break;
        }
        case OP_EQUAL:
        {
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(valuesEqual(a, b)));
            break;
        }
        case OP_GREATER:
        {
            BINARY_OP(BOOL_VAL, >);
            break;
        }
        case OP_LESS:
        {
            BINARY_OP(BOOL_VAL, <);
            break;
        }
        case OP_ADD:
        {
            if (IS_STRING(peek(0)) && IS_STRING(peek(1)))
            {
                concatenate();
            }
            else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1)))
            {
                ZReal64 b = AS_NUMBER(pop());
                ZReal64 a = AS_NUMBER(pop());
                push(NUMBER_VAL(a + b));
            }
            else
            {
                runtimeError(
                    "Les opérandes doivent être deux nombres ou deux chaînes."
                );
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_SUBTRACT:
        {
            BINARY_OP(NUMBER_VAL, -);
            break;
        }
        case OP_MULTIPLY:
        {
            BINARY_OP(NUMBER_VAL, *);
            break;
        }
        case OP_DIVIDE:
        {
            BINARY_OP(NUMBER_VAL, /);
            break;
        }
        case OP_NOT:
        {
            push(BOOL_VAL(isFalsey(pop())));
            break;
        }
        case OP_NEGATE:
        {
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("L'opérande doit être un nombre.");
                return INTERPRET_RUNTIME_ERROR;
            }
            
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        }
        case OP_PRINT:
        {
            printValue(pop());
            printf("\n");
            break;
        }
        case OP_JUMP:
        {
            ZUInt16 offset = READ_24BIT_OFFSET();
            frame->ip += offset;
            break;
        }
        case OP_JUMP_IF_FALSE:
        {
            ZUInt16 offset = READ_24BIT_OFFSET();
            if (isFalsey(peek(0)))
            {
                frame->ip += offset;
            }
            break;
        }
        case OP_LOOP:
        {
            ZUInt16 offset = READ_24BIT_OFFSET();
            frame->ip -= offset;
            break;
        }
        case OP_MODULO:
        {
            if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
                runtimeError("Les opérandes doivent être des nombres.");
                return INTERPRET_RUNTIME_ERROR;
            }
            ZReal64 b = AS_NUMBER(pop());
            ZReal64 a = AS_NUMBER(pop());
            if (b == 0) {
                runtimeError("Division par zéro.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(ziaFmod(a, b)));
            break;
        }
        case OP_DUP:
        {
            push(peek(0));
            break;
        }
        case OP_INCREMENT:
        {
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("L'opérande doit être un nombre.");
                return INTERPRET_RUNTIME_ERROR;
            }

            push(NUMBER_VAL(AS_NUMBER(pop()) + 1));
            break;
        }
        case OP_DECREMENT:
        {
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("L'opérande doit être un nombre.");
                return INTERPRET_RUNTIME_ERROR;
            }

            push(NUMBER_VAL(AS_NUMBER(pop()) - 1));
            break;
        }
        case OP_RETURN:
        {
            //Exit Interpreter
            return INTERPRET_OK;
        }
        default:
            break;
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_SHORT
#undef READ_STRING
#undef BINARY_OP
}

InterpretResult interpret(const ZChar* source)
{
    ObjFunction* function = compile(source);
    if (NULL == function)
    {
        return INTERPRET_COMPILE_ERROR;
    }
        
    push(OBJ_VAL(function));
    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->function = function;
    frame->ip = function->chunk.code;
    frame->slots = vm.stack;

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

static Value peek(ZInt32 distance)
{
    return vm.stackTop[-1 - distance];
}

static ZBool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate()
{
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    ZInt32 length = a->length + b->length;
    ZChar* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars,  a->length);
    memcpy((chars + a->length), b->chars, b->length);
    chars[length] = NULL_CHAR;

    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}
