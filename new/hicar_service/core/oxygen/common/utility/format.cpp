#include "format.h"

#include <sstream>
#include <iomanip>

namespace hsae {
namespace oxygen {

std::string hex(const void * data, size_t size)
{
    std::ostringstream format;
    for(size_t index = 0; index < size; ++index) {
        format << std::hex << std::setfill('0') << std::setw(2)<< std::uppercase
               << (int)((*(((const uint8_t *)data) + index)) & 0xff);
        if (index + 1 != size) {
            format << " ";
        }
    }

    return format.str();
}

} // namespace oxygen
} // hsae
