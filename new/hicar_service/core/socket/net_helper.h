#pragma once

#include <array>
#include <string>

namespace hsae {

std::string interface_to_ip (const std::string & _dev);
std::array<uint8_t, 6> interface_to_mac (const std::string & _dev);

} // namespace hase
