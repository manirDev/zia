#include "memory.h"
#include <stdlib.h>


void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }
    
    void* result = realloc(pointer, newSize);
    if (NULL == result)
    {
        //TBD: handle case of out of memory before aborting the process
        exit(1);
    }
    
    return result;
}
