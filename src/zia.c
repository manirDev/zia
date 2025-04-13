#include "common/common.h"
#include "chunk/chunk.h"
#include "vm/vm.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl()
{
    char line[1024];
    for(;;)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        interpret(line);
        
    }
}

static char* readFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (NULL == fileno)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier \"%s\".\n", path);
        exit(74);
    }

    //Moves the file cursor to the end of the file so we can measure its size.
    fseek(file, 0L, SEEK_END);
    //Returns the current position of the cursor — which, since we are at the end, gives the total file size in bytes.
    size_t fileSize = ftell(file);
    //Moves the file cursor back to the beginning, so we can read from the start.
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (NULL == buffer)
    {
        fprintf(stderr, "Mémoire insuffisante pour lire \"%s\".\n", path);
        exit(74);
    }
    
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Impossible de lire le fichier \"%s\".\n", path);
        exit(74);
    }
    
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path)
{
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
    {
        exit(65);
    }
    if (result == INTERPRET_RUNUTIME_ERROR)
    {
        exit(70);
    }  
}

int main(int argc, const char* argv[])
{
    initVM();

    if (argc == 1)
    {
        repl();
    }
    else if(argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Utilisage: zia [path]\n");
        exit(64);
    }

    freeVM();

    return 0;
}