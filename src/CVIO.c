#include "CVIO.h"
#include <stdio.h>
#include <stdlib.h>



char *cvReadString(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    
    if (! file) {
        printf("Could not open the file %s \n", fileName);

        return NULL;
    }

    fseek(file, 0, SEEK_END);
    //Returns the position of the cursor in bytes
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* contents = (char*)malloc(fileSize + 1);
    fread(contents, sizeof(char), fileSize, file);

    contents[fileSize] = '\0';

    fclose(file);

    return contents;
}