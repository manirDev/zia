#ifndef ZAI_VALUE_H
#define ZAI_VALUE_H

#include "common/commonTypes.h"

#define IS_BOOL(value)      ((value).type == VAL_BOOL)
#define IS_NIL(value)       ((value).type == VAL_NUL)
#define IS_NUMBER(value)    ((value).type == VAL_NUMBER)
#define IS_OBJ(value)       ((value).type == VAL_OBJ)

#define AS_BOOL(value)      ((value).as.boolean)
#define AS_NUMBER(value)    ((value).as.number)
#define AS_OBJ(value)       ((value).as.obj)

#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.boolean = value}})
#define NUL_VAL             ((Value){VAL_NUL, {.number = 0}})
#define NUMBER_VAL(value)   ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object)     ((Value){VAL_OBJ, {.obj = (Obj*)object}})

typedef enum
{
    VAL_BOOL,
    VAL_NUL,
    VAL_NUMBER,
    VAL_OBJ
}ValueType;

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef struct
{
    ValueType type;
    union
    {
     ZBool boolean;
     ZReal64 number;
     Obj *obj;
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