#include "chunk.h"
#include <stdlib.h>
#include "memory/memory.h"

void initChunk(Chunk* chunk)
{
    chunk->lines = NULL;
    chunk->code = NULL;
    chunk->capacity = 0;
    chunk->count = 0;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk * chunk)
{
    FREE_ARRAY(ZUInt8, chunk->code, chunk->capacity);
    FREE_ARRAY(ZUInt32, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, ZUInt8 byte, ZInt32 line)
{
    if (chunk->capacity < (chunk->count + 1))
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(
            ZUInt8, 
            chunk->code,
            oldCapacity,
            chunk->capacity);
        chunk->lines = GROW_ARRAY(
            ZInt32, 
            chunk->lines,
            oldCapacity,
            chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

ZInt32 addConstant(Chunk* chunk,  Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
