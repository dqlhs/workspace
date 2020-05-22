#pragma once

#include "../log/oxygen_log.h"

#define THROW(exc) \
    do { ERROR(#exc); throw (exc); } while(0)

#define THROW_EXP(exp, exception) \
    do { if (!(exp)) {THROW(exception);} } while(0)
