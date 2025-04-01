#ifndef ZIA_DEBUG_H
#define ZIA_DEBUG_H

#include "common/commonTypes.h"
#include "chunk/chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
ZInt32 disassembleInstruction(Chunk* chunk, ZInt32 offset);

#endif