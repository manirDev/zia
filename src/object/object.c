#include <stdio.h>
#include <string.h>

#include "memory/memory.h"
#include "object/object.h"
#include "value/value.h"
#include "vm/vm.h"
#include "table/table.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type)
{
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    object->isMarked = ZFALSE;

    object->next = vm.objects;
    vm.objects = object;

#ifdef DEBUG_LOG_GC
    printf("%p allocate %zu for %d\n", (void*)object, size, type);
#endif

    return object;
}

ObjClosure *newClosure(ObjFunction *function)
{
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);
    for (ZInt32 i = 0; i < function->upvalueCount; i++)
    {
        upvalues[i] = NULL;
    }

    ObjClosure *closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjFunction *newFunction()
{
    ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
}

ObjNativeFn *newNative(NativeFn function)
{
    ObjNativeFn *nativefn = ALLOCATE_OBJ(ObjNativeFn, OBJ_NATIVE);
    nativefn->function = function;
    return nativefn;
}

static ZUInt32 hashString(const ZChar *key, ZInt32 length)
{
    ZUInt32 hash = 2166136261u;
    for (ZInt32 i = 0; i < length; i++)
    {
        hash ^= (ZUInt32)key[i];
        hash *= 16777619;
    }
    return hash;
}

static ObjString *allocateString(ZChar *chars, ZInt32 length, ZUInt32 hash)
{
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    push(OBJ_VAL(string));
    tableSet(&vm.strings, string, NUL_VAL);
    pop();
    
    return string;
}

ObjString *takeString(ZChar *chars, ZInt32 length)
{
    ZUInt32 hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (NULL != interned)
    {
        FREE_ARRAY(ZChar, chars, length + 1);
        return interned;
    }
    return allocateString(chars, length, hash);
}

ObjString *copyString(const ZChar *chars, ZInt32 length)
{
    ZUInt32 hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (NULL != interned)
    {
        return interned;
    }

    ZChar *heapChars = ALLOCATE(ZChar, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = NULL_CHAR;
    return allocateString(heapChars, length, hash);
}

ObjUpvalue *newUpvalue(Value *slot)
{
    ObjUpvalue *upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->closed = NUL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

static void printFunction(ObjFunction *function)
{
    if (NULL == function->name)
    {
        printf("<script>");
        return;
    }

    printf("<fn %s>", function->name->chars);
}

void printObject(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_CLOSURE:
    {
        printFunction(AS_CLOSURE(value)->function);
        break;
    }
    case OBJ_FUNCTION:
        printFunction(AS_FUNCTION(value));
        break;
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    case OBJ_NATIVE:
        printf("<native fn>");
        break;
    case OBJ_UPVALUE:
        printf("upvalue");
        break;
    }
}
