#ifndef _ASTEROIDS_LOG_HPP_
#define _ASTEROIDS_LOG_HPP_

namespace asteroids
{
    class Log
    {
    public:
        static void error(const char* pMessage, ...);
        static void warn(const char* pMessage, ...);
        static void info(const char* pMessage, ...);
        static void debug(const char* pMessage, ...);
    };
}

#ifndef NDEBUG
    #define asteroids_Log_debug(...) asteroids::Log::debug(__VA_ARGS__)
#else
    #define asteroids_Log_debug(...)
#endif

#endif
