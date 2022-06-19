#include "Log.h"
#include <android/log.h>

void Log::debug(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    __android_log_vprint(ANDROID_LOG_DEBUG, "PortableGL", format, args);
    va_end(args);
#else
    (void)format;
#endif
}

void Log::info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    __android_log_vprint(ANDROID_LOG_INFO, "PortableGL", format, args);
    va_end(args);
}

void Log::warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    __android_log_vprint(ANDROID_LOG_WARN, "PortableGL", format, args);
    va_end(args);
}

void Log::error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    __android_log_vprint(ANDROID_LOG_ERROR, "PortableGL", format, args);
    va_end(args);
}
