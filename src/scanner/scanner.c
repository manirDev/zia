#include "scanner.h"
#include "common/common.h"
#include "common/commonTypes.h"
#include <stdio.h>
#include <string.h>

typedef struct
{
    const char* start;
    const char* current;
    ZInt32 line;
}Scanner;

Scanner scanner;

void initScanner(const char* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}