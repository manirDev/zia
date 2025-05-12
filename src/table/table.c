#include <stdlib.h>
#include <string.h>

#include "memory/memory.h"
#include "object/object.h"
#include "table/table.h"
#include "value/value.h"

#define TBALE_MAX_LOAD 0.75

void initTable(Table* table)
{
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table)
{
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

static Entry* findEntry(Entry* entries, ZInt32 capacity, ObjString* key)
{
    ZUInt32 index = key->hash % capacity;
    Entry* tombStone = NULL;
    for (;;)
    {
        Entry* entry = &entries[index];
        if (NULL == entry->key)
        {
            if (IS_NIL(entry->value))
            {
                return NULL != tombStone ? tombStone : entry;
            }
            else
            {
                if (NULL == tombStone)
                {
                    tombStone = entry;
                }
                
            }
        }
        else if (entry->key == key)
        {
           return entry;
        }

        index = (index + 1) % capacity;
    }
    
}

ZBool tableGet(Table* table, ObjString* key, Value* value)
{   
    if (table->count == 0)
    {
        return ZFALSE;
    }
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (NULL == entry->key)
    {
        return ZFALSE;
    }
    
    *value = entry->value;
    return ZTRUE;
}

static void adjustCapacity(Table* table, ZInt32 capacity)
{
    Entry* entries = ALLOCATE(Entry, capacity);
    for (ZInt32 i = 0; i < capacity; i++)
    {
        entries[i].key = NULL;
        entries[i].value = NUL_VAL;
    }

    table->count = 0;
    for (ZInt32 i = 0; i < table->capacity; i++)
    {
        Entry* entry = &table->entries[i];
        if (NULL == entry->key)
        {
            continue;
        }
        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }
    
    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

ZBool tableSet(Table* table, ObjString* key, Value value)
{
    /*
    This is how we manage the table’s load factor. 
    We don’t grow when the capacity is completely full. 
    Instead, we grow the array before then, when the array 
    becomes at least TBALE_MAX_LOAD(75% for example) full.
    */
    if (table->count + 1 > table->capacity + TBALE_MAX_LOAD) 
    {
        ZInt32 capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }
    
    Entry* entry = findEntry(table->entries, table->capacity, key);
    ZBool isNewKey = NULL == entry->key;
    if (isNewKey && IS_NIL(entry->value))
    {
        table->count++;
    }
    entry->key = key;
    entry->value = value;
    return isNewKey;
}

ZBool tableDelete(Table* table, ObjString* key)
{
    if (table->count == 0)
    {
        return ZFALSE;
    }
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (NULL == entry)
    {
        return ZFALSE;
    }
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return ZTRUE;
}

void tableAddAll(Table* from, Table* to)
{
    for (ZInt32 i = 0; i < from->capacity; i++)
    {
        Entry* entry = &from->entries[i];
        if (NULL != entry->key)
        {
           tableSet(to, entry->key, entry->value);
        }
    }
    
}

ObjString* tableFindString(Table* table, const ZChar* chars, ZInt32 length, ZUInt32 hash)
{
    if (table->count == 0)
    {
        return NULL;
    }
    
    ZUInt32 index = hash % table->capacity;
    for(;;)
    {
        Entry* entry = &table->entries[index];
        if (NULL == entry->key)
        {
            if(IS_NIL(entry->value)) 
            {
                return NULL;
            }
        }
        else if (entry->key->length == length && entry->key->hash == hash
                && memcmp(entry->key->chars, chars, length) == 0)
        {
            return entry->key;
        }
        
        index = (index + 1) % table->capacity;
    }
}
