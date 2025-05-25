#ifndef ZIA_CHUNCK_H
#define ZIA_CHUNCK_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "value/value.h"

typedef enum
{
    OP_CONSTANT,
    OP_NULL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_DEFINE_GLOBAL,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_RETURN,
}OpCode;

typedef struct
{
    ZInt32 count;
    ZInt32 capacity;
    ZUInt8* code;
    ZInt32* lines;
    ValueArray constants;
}Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, ZUInt8 byte, ZInt32 line);
void freeChunk(Chunk * chunk);
ZInt32 addConstant(Chunk* chunk,  Value value);

#endif