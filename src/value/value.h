#ifndef ZAI_VALUE_H
#define ZAI_VALUE_H

#include "common/commonTypes.h"

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NUL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.boolean = value}})
#define NUL_VAL             ((Value){VAL_NUL, {.number = 0}})
#define NUMBER_VAL(value)   ((Value){VAL_NUMBER, {.number = value}})

typedef enum
{
    VAL_BOOL,
    VAL_NUL,
    VAL_NUMBER,
}ValueType;

typedef struct
{
    ValueType type;
    union
    {
     ZBool boolean;
     ZReal64 number;
    }as;
    
}Value;

typedef struct
{
    ZInt32 capacity;
    ZInt32 count;
    Value *values;
}ValueArray;

ZBool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif