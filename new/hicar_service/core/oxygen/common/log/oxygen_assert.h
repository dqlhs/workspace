#pragma once

#include <cassert>

#include "oxygen_log.h"

#define OXYGEN_ASSERT(x)   \
        do {               \
            if (!(x)) {      \
                ERROR(#x); \
                abort();   \
            }\
        } while(0)
