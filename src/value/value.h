#ifndef ZAI_VALUE_H
#define ZAI_VALUE_H

#include "common/commonTypes.h"

typedef ZReal64 Value;

typedef struct
{
    ZInt32 capacity;
    ZInt32 count;
    Value *values;
}ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif