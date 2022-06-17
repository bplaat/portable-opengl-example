#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t *fileRead(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf("Can't load file: %s\n", path);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t *buffer = (uint8_t *)malloc(fileSize + 1);
    fileSize = fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);
    return buffer;
}
