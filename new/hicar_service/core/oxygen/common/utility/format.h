#pragma once

#include <string>

namespace hsae {
namespace oxygen {

std::string hex(const void * data, size_t size);

template <typename IterData>
std::string hex(IterData && data)
{
    return hex(data.data(), data.size());
}

} // namespace oxygen
} // hsae
