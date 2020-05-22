#pragma once

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace hsae {
namespace oxygen {
enum LEVEL {
    LEVEL_TRACE = 0,
    LEVEL_INFO  = 1,
    LEVEL_WARN  = 2,
    LEVEL_ERROR = 3,
    LEVEL_FATAL = 4
};

enum DEBUG_SWITCH {
    DEBUG_CLOSE = 0,
    DEBUG_OPEN  = 1
};

int level();

int debug();

uint64_t timestamp();

} // namespace oxygen
} // hsae



// [ time ] [ file ] [ line ] [ func ] : message

#ifdef TRACE
# undef TRACE
#endif

#ifdef INFO
# undef INFO
#endif

#ifdef WARN
# undef WARN
#endif

#ifdef ERROR
# undef ERROR
#endif

#ifdef FATAL
# undef FATAL
#endif

#ifdef FLUSH
# undef FLUSH
#endif

#define FLUSH() fflush(stdout)

#define TRACE(msg, arg...) \
    do { \
        if (hsae::oxygen::debug() == hsae::oxygen::DEBUG_CLOSE) break; \
        if (hsae::oxygen::level() > hsae::oxygen::LEVEL_TRACE) break; \
        printf("[TRACE][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define INFO(msg, arg...) \
    do { \
        if (hsae::oxygen::debug() == hsae::oxygen::DEBUG_CLOSE) break; \
        if (hsae::oxygen::level() > hsae::oxygen::LEVEL_INFO) break; \
        printf("[ INFO][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define WARN(msg, arg...) \
    do { \
        if (hsae::oxygen::debug() == hsae::oxygen::DEBUG_CLOSE) break; \
        if (hsae::oxygen::level() > hsae::oxygen::LEVEL_WARN) break; \
        printf("[ WARN][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define ERROR(msg, arg...) \
    do { \
        if (hsae::oxygen::debug() == hsae::oxygen::DEBUG_CLOSE) break; \
        if (hsae::oxygen::level() > hsae::oxygen::LEVEL_ERROR) break; \
        printf("[ERROR][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define FATAL(msg, arg...) \
    do { \
        if (hsae::oxygen::debug() == hsae::oxygen::DEBUG_CLOSE) break; \
        if (hsae::oxygen::level() > hsae::oxygen::LEVEL_FATAL) break; \
        printf("[FATAL][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
    } while(0)

#define ABORT(msg, arg...)\
    do { \
        printf("[ABORT][%" PRIu64 "][%s][%s][%d] -> {" msg "}\r\n", hsae::oxygen::timestamp(), __FILENAME__, __func__, __LINE__, ##arg); \
        FLUSH(); \
        abort(); \
    } while(0)
