#include "vm.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "debug.h"
#include "object/object.h"
#include "memory/memory.h"
#include "compiler/compiler.h"

VM vm;

static Value peek(ZInt32 distance);
static ZBool isFalsey(Value value);
static void concatenate();
static ZBool call(ObjClosure* closure, ZInt32 argCount);
static ZBool callValue(Value callee, ZInt32 argCount);
static ObjUpvalue* captureUpvalue(Value* local);
static void closeUpvalues(Value* last);

static Value clockNative(ZInt32 argCount, Value* args)
{
    return NUMBER_VAL((ZReal64)clock() / CLOCKS_PER_SEC);
}

static void resetStack()
{
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = NULL;
}

static void runtimeError(const ZChar *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    for (ZInt32 i = vm.frameCount - 1; i >= 0; i--)
    {
        CallFrame* frame = &vm.frames[i];
        ObjFunction* function = frame->closure->function;
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[ligne %d] dans ", function->chunk.lines[instruction]);
        if (NULL == function->name)
        {
           fprintf(stderr, "script\n");
        }
        else
        {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    
    resetStack();
}

static void defineNative(const ZChar* name, NativeFn function)
{
    push(OBJ_VAL(copyString(name, (ZInt32)strlen(name))));
    push(OBJ_VAL(newNative(function)));
    tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}

static ZBool isInteger(ZReal64 exponent)
{
    return ((ZInt32)exponent == exponent);
}

static ZReal64 ziaFmod(ZReal64 a, ZReal64 b)
{
    ZReal64 quotient = (a / b);
    ZInt32 truncated = (ZInt32)quotient;
    return (a - (b * truncated));
}

static InterpretResult ziaPow(ZReal64 base, ZReal64 exponent, ZReal64 *result)
{
    ZInt32 exp = (ZInt32)exponent;

    if (base == 0.0 && exp < 0)
    {
        runtimeError("Division par zéro.");
        return INTERPRET_RUNTIME_ERROR;
    }

    *result = 1.0;

    if (exp >= 0)
    {
        for (ZInt32 i = 0; i < exp; i++)
        {
            *result *= base;
        }
    }
    else
    {
        for (ZInt32 i = 0; i < -exp; i++)
        {
            *result *= base;
        }
        *result = 1.0 / (*result);
    }

    return INTERPRET_OK;
}

void initVM()
{
    resetStack();
    vm.objects = NULL;
    initTable(&vm.globals);
    initTable(&vm.strings);

    defineNative("temps", clockNative);
}

void freeVM()
{
    freeTable(&vm.globals);
    freeTable(&vm.strings);
    freeObjects();
}

static InterpretResult run()
{
    CallFrame *frame = &vm.frames[vm.frameCount - 1];

#define READ_BYTE() (*frame->ip++)
#define READ_CONSTANT() (frame->closure->function->chunk.constants.values[READ_BYTE()])
#define READ_SHORT() \
    (frame->ip += 2, (ZUInt16)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_24BIT()                   \
    (frame->ip += 3,                   \
     (ZUInt32)((frame->ip[-3] << 16) | \
               (frame->ip[-2] << 8) |  \
               frame->ip[-1]))
#define READ_24BIT_OFFSET() READ_24BIT()
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op)                                     \
    do                                                               \
    {                                                                \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))              \
        {                                                            \
            runtimeError("Les opérandes doivent être des nombres."); \
            return INTERPRET_RUNTIME_ERROR;                          \
        }                                                            \
        ZReal64 b = AS_NUMBER(pop());                                \
        ZReal64 a = AS_NUMBER(pop());                                \
        push(valueType(a op b));                                     \
    } while (ZFALSE)

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
        disassembleInstruction(&frame->closure->function->chunk, (ZInt32)(frame->ip - frame->closure->function->chunk.code));
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
            ObjString *name = READ_STRING();
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
            ObjString *name = READ_STRING();
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
            ObjString *name = READ_STRING();
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
                    "Les opérandes doivent être deux nombres ou deux chaînes.");
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
        case OP_JUMP_IF_TRUE:
        {
            ZUInt16 offset = READ_24BIT_OFFSET();
            if (!isFalsey(peek(0)))
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
            if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
            {
                runtimeError("Les opérandes doivent être des nombres.");
                return INTERPRET_RUNTIME_ERROR;
            }
            ZReal64 b = AS_NUMBER(pop());
            ZReal64 a = AS_NUMBER(pop());
            if (b == 0)
            {
                runtimeError("Division par zéro.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(ziaFmod(a, b)));
            break;
        }
        case OP_POWER:
        {
            if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))
            {
                runtimeError("Les opérandes doivent être des nombres.");
                return INTERPRET_RUNTIME_ERROR;
            }
            ZReal64 exponent = AS_NUMBER(pop());
            ZReal64 base = AS_NUMBER(pop());

            if (!isInteger(exponent))
            {
                runtimeError("L'exposant doit être un entier.");
                return INTERPRET_RUNTIME_ERROR;
            }

            ZReal64 result = 1;
            InterpretResult retCode = ziaPow(base, exponent, &result);
            if (INTERPRET_RUNTIME_ERROR == retCode)
            {
                return retCode;
            }

            push(NUMBER_VAL(result));
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
        case OP_SWITCH:
        case OP_CASE:
        case OP_DEFAULT:
        {
            runtimeError("Unsupported legacy switch opcode");
            return INTERPRET_RUNTIME_ERROR;
        }
        case OP_SWAP:
        {
            Value a = pop();
            Value b = pop();
            push(a);
            push(b);
            break;
        }
        case OP_CALL:
        {
            ZInt32 argCount = READ_BYTE();
            if (!callValue(peek(argCount), argCount))
            {
                return INTERPRET_RUNTIME_ERROR;
            }
            frame = &vm.frames[vm.frameCount - 1];
            break;
        }
        case OP_CLOSURE:
        {
            ObjFunction* function = AS_FUNCTION(READ_CONSTANT());
            ObjClosure* closure = newClosure(function);
            push(OBJ_VAL(closure));
            for (ZInt32 i = 0; i < closure->upvalueCount; i++)
            {
                ZUInt8 isLocal = READ_BYTE();
                ZUInt8 index = READ_BYTE();
                if (ZTRUE == isLocal)
                {
                    closure->upvalues[i] = captureUpvalue(frame->slots + index);
                }
                else
                {
                    closure->upvalues[i] = frame->closure->upvalues[index];
                }
            }
            
            break;
        }
        case OP_GET_UPVALUE:
        {
            ZUInt8 slot = READ_BYTE();
            push(*frame->closure->upvalues[slot]->location);
            break;
        }
        case OP_SET_UPVALUE:
        {
            ZUInt8 slot = READ_BYTE();
            *frame->closure->upvalues[slot]->location = peek(0);
            break;
        }
        case OP_CLOSE_UPVALUE:
        {
            closeUpvalues(vm.stackTop - 1);
            pop();
            break;
        }
        case OP_RETURN:
        {
            Value result = pop();
            closeUpvalues(frame->slots);
            vm.frameCount--;
            if (vm.frameCount == 0)
            {
                pop();
                return INTERPRET_OK;
            }
            
            vm.stackTop = frame->slots;
            push(result);
            frame = &vm.frames[vm.frameCount - 1];
            break;
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
#undef READ_24BIT
#undef READ_24BIT_OFFSET
}

InterpretResult interpret(const ZChar *source)
{
    ObjFunction *function = compile(source);
    if (NULL == function)
    {
        return INTERPRET_COMPILE_ERROR;
    }

    push(OBJ_VAL(function));
    ObjClosure* closure = newClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

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

static ZBool call(ObjClosure* closure, ZInt32 argCount)
{
    if (argCount != closure->function->arity)
    {
        runtimeError("Attendu %d arguments mais %d ont été fournis.", closure->function->arity, argCount);
        return ZFALSE;  
    }
    
    if (vm.frameCount == FRAMES_MAX)
    {
        runtimeError("Stack Overflow");
        return ZFALSE;
    }
    

    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frame->slots = vm.stackTop - argCount - 1;
    return ZTRUE;
}

static ZBool callValue(Value callee, ZInt32 argCount)
{
    if (IS_OBJ(callee))
    {
        switch (OBJ_TYPE(callee))
        {
        case OBJ_CLOSURE:
            return call(AS_CLOSURE(callee), argCount);
        case OBJ_NATIVE:
        {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(argCount, vm.stackTop - argCount);
            vm.stackTop -= argCount + 1;
            push(result);
            return ZTRUE;
        }
        default:
            break;
        }
    }
     
    runtimeError("Seules les fonctions et les classes peuvent être appelées.");

    return ZFALSE;
}

static ObjUpvalue* captureUpvalue(Value* local)
{
    ObjUpvalue* prevUpvalue = NULL;
    ObjUpvalue* upvalue = vm.openUpvalues;

    while (NULL != upvalue && upvalue->location > local)
    {
        prevUpvalue = upvalue;
        upvalue = upvalue->next;
    }
    
    if (NULL != upvalue && upvalue->location == local)
    {
        return upvalue;
    }
    
    ObjUpvalue* createdUpvalue = newUpvalue(local);
    createdUpvalue->next = upvalue;

    if (NULL == prevUpvalue)
    {
        vm.openUpvalues = createdUpvalue;
    }
    else
    {
        prevUpvalue->next = createdUpvalue;
    }

    return createdUpvalue;
}

static void closeUpvalues(Value* last)
{
    while(NULL != vm.openUpvalues && vm.openUpvalues->location >= last)
    {
        ObjUpvalue* upvalue = vm.openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm.openUpvalues = upvalue->next;
    }
}

static ZBool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate()
{
    ObjString *b = AS_STRING(pop());
    ObjString *a = AS_STRING(pop());

    ZInt32 length = a->length + b->length;
    ZChar *chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy((chars + a->length), b->chars, b->length);
    chars[length] = NULL_CHAR;

    ObjString *result = takeString(chars, length);
    push(OBJ_VAL(result));
}
