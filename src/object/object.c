#include <stdio.h>
#include <string.h>

#include "memory/memory.h"
#include "object/object.h"
#include "value/value.h"
#include "vm/vm.h"

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

static ObjString* allocateString(ZChar* chars, ZInt32 length)
{
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    return string;
}

ObjString* takeString(ZChar* chars, ZInt32 length)
{
    return allocateString(chars, length);
}

ObjString* copyString(const ZChar* chars, ZInt32 length)
{
    ZChar* heapChars = ALLOCATE(ZChar, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = NULL_CHAR;
    return allocateString(heapChars, length);
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
