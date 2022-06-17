#include "Log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Log::debug(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    printf("[DEBUG] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
#endif
}

void Log::info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[INFO] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void Log::warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "[WARNING] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void Log::error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}
