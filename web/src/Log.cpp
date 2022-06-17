#include "Log.h"
#include "std.h"
#include <stdarg.h>

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
    printf("[WARNING] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void Log::error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[ERROR] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
    exit(EXIT_FAILURE);
}
