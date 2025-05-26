#ifndef ZAI_OBJECT_H
#define ZAI_OBJECT_H

#include "common/commonTypes.h"
#include "value/value.h"
#include "chunk/chunk.h"

#define OBJ_TYPE(value)     (AS_OBJ(value)->type)

#define IS_FUNCTION(value)  isObjType(value, OBJ_FUNCTION)
#define IS_STRING(value)    isObjType(value, OBJ_STRING)

#define AS_FUNCTION(value)  ((ObjFunction*)AS_OBJ(value))
#define AS_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)   (((ObjString*)AS_OBJ(value))->chars)


typedef enum
{
    OBJ_FUNCTION,
    OBJ_STRING,
}ObjType;

struct Obj
{
    ObjType type;
    struct Obj* next;
};

typedef struct
{
    Obj obj;
    ZInt32 arity; //stores the number of parameters the function expects
    Chunk chunk;
    ObjString* name;
}ObjFunction;

struct ObjString
{
    Obj obj;
    ZInt32 length;
    ZChar* chars;
    ZUInt32 hash;
};

ObjFunction* newFunction();
ObjString* takeString(ZChar* chars, ZInt32 length);
ObjString* copyString(const ZChar* chars, ZInt32 length);
void printObject(Value value);

static inline ZBool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
