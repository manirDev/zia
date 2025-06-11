#ifndef ZIA_MEMORY_H
#define ZIA_MEMORY_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "object/object.h"


#define INIT_CAPACITY   0x0100
#define CAPACITY_FACTOR 0x0002

#define ALLOCATE(type, count) \
        (type*)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(capacity) \
        ((capacity) < INIT_CAPACITY ? INIT_CAPACITY : (capacity) * CAPACITY_FACTOR)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
        (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
        reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);
void markObject(Obj* object);
void markValue(Value value);
void collectGarbage();
void freeObjects();

#endif