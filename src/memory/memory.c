#include "memory/memory.h"
#include "vm/vm.h"
#include <stdlib.h>


void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }
    
    void* result = realloc(pointer, newSize);
    if (NULL == result)
    {
        //TBD: handle case of out of memory before aborting the process
        exit(1);
    }
    
    return result;
}

static void freeObject(Obj* object)
{
    switch (object->type)
    {
    case OBJ_CLOSURE:
    {
        ObjClosure* closure = (ObjClosure*)object;
        FREE_ARRAY(ObjUpvalue*, closure->upvalues, closure->upvalueCount);
        FREE(ObjClosure, object);
        break;
    }
    case OBJ_FUNCTION:
    {
        ObjFunction* function = (ObjFunction*)object;
        freeChunk(&function->chunk);
        FREE(ObjFunction, object);
        break;
    }
    case OBJ_NATIVE:
    {
        FREE(ObjNativeFn, object);
        break;
    }
    case OBJ_STRING:
    {
        ObjString* string = (ObjString*)object;
        FREE_ARRAY(char, string->chars, string->length + 1);
        FREE(ObjString, object);
        break;
    }
    case OBJ_UPVALUE:
    {
        FREE(ObjUpvalue, object);
    }
    default:
        break;
    }
}

void freeObjects()
{
    Obj* object = vm.objects;
    while (NULL != object)
    {
       Obj* next = object->next;
       freeObject(object);
       object = next;
    }
    
}
