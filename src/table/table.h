#ifndef ZAI_TABLE_H
#define ZAI_TABLE_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "value/value.h"

typedef struct
{
    ObjString* key;
    Value value;
}Entry;

typedef struct
{
   ZInt32 count;
   ZInt32 capacity;
   Entry* entries;
}Table;

void initTable(Table* table);
void freeTable(Table* table);
ZBool tableGet(Table* table, ObjString* key, Value* value);
ZBool tableSet(Table* table, ObjString* key, Value value);
ZBool tableDelete(Table* table, ObjString* key);
void tableAddAll(Table* from, Table* to);
ObjString* tableFindString(Table* table, const ZChar* chars, ZInt32 length, ZUInt32 hash);
void tableRemoveWhite(Table* table);
void markTable(Table* table);

#endif
