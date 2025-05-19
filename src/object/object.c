#include <stdio.h>
#include <string.h>

#include "memory/memory.h"
#include "object/object.h"
#include "value/value.h"
#include "vm/vm.h"
#include "table/table.h"

#define ALLOCATE_OBJ(type, objectType) \
        (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type)
{
    Obj* object = (Obj*)reallocate(NULL, 0,  size);
    object->type = type;

    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static ZUInt32 hashString(const ZChar* key, ZInt32 length)
{
    ZUInt32 hash = 2166136261u;
    for (ZInt32 i = 0; i < length; i++)
    {
        hash ^= (ZUInt32)key[i];
        hash *= 16777619;
    }
    return hash;
}

static ObjString* allocateString(ZChar* chars, ZInt32 length, ZUInt32 hash)
{
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    tableSet(&vm.strings, string, NUL_VAL);
    return string;
}

ObjString* takeString(ZChar* chars, ZInt32 length)
{
    ZUInt32 hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (NULL != interned)
    {
        FREE_ARRAY(ZChar, chars, length + 1);
        return interned;
    }
    return allocateString(chars, length, hash);
}

ObjString* copyString(const ZChar* chars, ZInt32 length)
{
    ZUInt32 hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (NULL != interned)
    {
       return interned;
    }
    
    ZChar* heapChars = ALLOCATE(ZChar, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = NULL_CHAR;
    return allocateString(heapChars, length, hash);
}

void printObject(Value value)
{
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    }
}
