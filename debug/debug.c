#include "debug.h"
#include <stdio.h>
#include "value/value.h"

void disassembleChunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name);

    for (ZInt32 offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
    
}

static ZInt32 simpleInstruction(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static ZInt32 constantInstruction(const char* name, Chunk* chunk, ZInt32 offset)
{
    ZUInt8 constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name,  constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

ZInt32 disassembleInstruction(Chunk* chunk, ZInt32 offset)
{
    printf("%04d ", offset);
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        printf(" | ");
    }
    else
    {
        printf("%4d ", chunk->lines[offset]);
    }

    ZUInt8 instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVISION", offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}