#ifndef ZAI_COMPILER_H
#define ZAI_COMPILER_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "chunk/chunk.h"
#include "object/object.h"

ZBool compile(const ZChar* source, Chunk* chunk);

#endif
