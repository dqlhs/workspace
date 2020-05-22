#include "oxygen_log.h"

#include <stdlib.h>
#include <time.h>

#ifdef  LEVEL
# undef LEVEL
#endif

#define LEVEL "PRINT_LEVEL"

#ifdef DEBUG_SWITCH
# undef DEBUG_SWITCH
#endif

#define DEBUG_SWITCH "DEBUG_SWITCH"

namespace hsae {
namespace oxygen {
int level()
{
    static const char * _level = getenv(LEVEL);
    if (_level != NULL) {
        return atoi(_level);
    } else {
        return LEVEL_INFO;
    }
}

int debug()
{
    static const char * _debug = getenv(DEBUG_SWITCH);
    if (_debug != NULL) {
        return atoi(_debug);
    } else {
        return DEBUG_OPEN;
    }
}

uint64_t timestamp()
{
    uint64_t time;
    timespec ts = {0};
    clock_gettime(CLOCK_MONOTONIC, &ts);

    time  = ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
    return time;
}

} // namespace oxygen
} // hsae

