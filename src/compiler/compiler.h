#ifndef ZAI_COMPILER_H
#define ZAI_COMPILER_H

#include "common/common.h"
#include "common/commonTypes.h"
#include "chunk/chunk.h"
#include "object/object.h"

#define JUMP_OFFSET_SIZE 3  // Use 24-bit offsets

ObjFunction* compile(const ZChar* source);
void markCompilerRoots();

#endif
