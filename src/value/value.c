#include "value.h"
#include <stdio.h>
#include <string.h>
#include "object/object.h"
#include "memory/memory.h"

void initValueArray(ValueArray* array)
{
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value)
{
    if (array->capacity < (array->count + 1))
    {
        ZInt32 oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(
            Value, 
            array->values,
            oldCapacity,
            array->capacity);
    }
    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

void printValue(Value value)
{
    switch (value.type)
    {
    case VAL_BOOL:
        printf(AS_BOOL(value) ? "vrai" : "faux");
        break;
    case VAL_NUL:
        printf("nul");
        break;
    case VAL_NUMBER:
        printf("%g", AS_NUMBER(value));
        break;
    case VAL_OBJ:
        printObject(value);
        break;
    }
}

ZBool valuesEqual(Value a, Value b)
{
    if (a.type != b.type)
    {
        return false;
    }
    switch (a.type)
    {
    case VAL_BOOL:
        return AS_BOOL(a) == AS_BOOL(b);
        break;
    case VAL_NUL:
        return true;
        break;
    case VAL_NUMBER:
        return AS_NUMBER(a) == AS_NUMBER(b);
        break;
    case VAL_OBJ:
    {
        ObjString* aString = AS_STRING(a);
        ObjString* bString = AS_STRING(b);
        return (aString->length == bString->length
                && memcmp(aString->chars, bString->chars, 
                          aString->length
                         )
               );
    }
    default:
        return false;
    }
}
