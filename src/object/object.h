#ifndef ZAI_OBJECT_H
#define ZAI_OBJECT_H

#include "common/commonTypes.h"
#include "value/value.h"

#define OBJ_TYPE(value)     (AS_OBJ(value)->type)
#define IS_STRING(value)    isObjType(value, OBJ_STRING)

#define AS_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)    (((ObjString*)AS_OBJ(value))->chars)


typedef enum
{
    OBJ_STRING,
}ObjType;

struct Obj
{
    ObjType type;
};

struct ObjString
{
    Obj obj;
    ZInt32 length;
    ZChar* chars;
};

ObjString* takeString(ZChar* chars, ZInt32 length);
ObjString* copyString(const ZChar* chars, ZInt32 length);
void printObject(Value value);

static inline ZBool isObjType(Value value, ObjType type)
{
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
