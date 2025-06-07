#include "debug.h"
#include <stdio.h>
#include "value/value.h"

void disassembleChunk(Chunk* chunk, const ZChar* name)
{
    printf("== %s ==\n", name);

    for (ZInt32 offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
    
}

static ZInt32 simpleInstruction(const ZChar* name, ZInt32 offset)
{
    printf("%s\n", name);
    return offset + 1;
}

static ZInt32 byteInstruction(const ZChar* name, Chunk* chunk, ZInt32 offset)
{
    ZUInt8 slot = chunk->code[offset + 1];
    printf("%-16s %4d\n", name, slot);
    return offset + 2;
}

static ZInt32 jumpInstruction(const ZChar* name, ZInt32 sign, Chunk* chunk, ZInt32 offset)
{
    ZUInt16 jump = (ZUInt16)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
    return offset + 3;
}

static ZInt32 constantInstruction(const ZChar* name, Chunk* chunk, ZInt32 offset)
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
    case OP_NULL:
        return simpleInstruction("OP_NULL", offset);
    case OP_TRUE:
        return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
    case OP_POP:
        return simpleInstruction("OP_POP", offset);
    case OP_SET_LOCAL:
        return byteInstruction("OP_SET_LOCAL", chunk, offset);
    case OP_GET_LOCAL:
        return byteInstruction("OP_GET_LOCAL", chunk, offset);
    case OP_SET_GLOBAL:
        return constantInstruction("OP_SET_GLOBAL", chunk, offset);
    case OP_GET_GLOBAL:
        return constantInstruction("OP_GET_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL:
        return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
    case OP_EQUAL:
        return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
        return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
        return simpleInstruction("OP_LESS", offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVISION", offset);
    case OP_NOT:
        return simpleInstruction("OP_NOT", offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_PRINT:
        return simpleInstruction("OP_PRINT", offset);
    case OP_JUMP:
        return jumpInstruction("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
        return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
        return jumpInstruction("OP_LOOP", -1, chunk, offset);
    case OP_MODULO:
        return simpleInstruction("OP_MODULO", offset);
    case OP_DUP:
        return simpleInstruction("OP_DUP", offset);
    case OP_INCREMENT:
        return simpleInstruction("OP_INCREMENT", offset);
    case OP_DECREMENT:
        return simpleInstruction("OP_DECREMENT", offset);
    case OP_CALL:
        return byteInstruction("OP_CALL", chunk, offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}