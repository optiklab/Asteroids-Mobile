#include "Log.h"

#include <stdarg.h>
#include <android/log.h>

namespace asteroids
{
    void Log::info(const char* pMessage, ...)
    {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_INFO, "ASTEROIDS", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_INFO, "ASTEROIDS", "\n");
        va_end(lVarArgs);
    }

    void Log::error(const char* pMessage, ...)
    {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_ERROR, "ASTEROIDS", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_ERROR, "ASTEROIDS", "\n");
        va_end(lVarArgs);
    }

    void Log::warn(const char* pMessage, ...)
    {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_WARN, "ASTEROIDS", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_WARN, "ASTEROIDS", "\n");
        va_end(lVarArgs);
    }

    void Log::debug(const char* pMessage, ...)
    {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_DEBUG, "ASTEROIDS", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_DEBUG, "ASTEROIDS", "\n");
        va_end(lVarArgs);
    }
}
