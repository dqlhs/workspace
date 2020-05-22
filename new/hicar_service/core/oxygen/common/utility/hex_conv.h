#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>

#include "exception/throw.h"

namespace hsae {
namespace oxygen {
uint8_t hex_to_uint8 (char c1, char c2);

std::vector<uint8_t> hex_to_vec(const std::string & _string,
                                std::size_t _pos = 0,
                                std::size_t _size = std::string::npos);

uint8_t hex_to_uint8(const std::array<char, 2> & _data);

uint16_t hex_to_uint16(const std::array<char, 4> & _data);

uint8_t hex_2byte_to_uint8 (const std::string & _string, std::size_t _pos = 0);

uint16_t hex_4byte_to_uint16 (const std::string & _string, std::size_t _pos = 0);

uint32_t hex_6byte_to_uint32 (const std::string & _string, std::size_t _pos = 0);

uint32_t hex_8byte_to_uint32 (const std::string & _string, std::size_t _pos = 0);

std::string value_to_hex_map(uint8_t _value);

std::string value_to_hex_map_without_fill_zero(uint8_t _value);

template <typename Integer, typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
std::string value_to_hex(Integer _value)
{
    std::size_t shift_count = sizeof (_value);

    std::string hex_string;
    for (; shift_count != 0; --shift_count) {
        uint8_t value = static_cast<uint8_t>(_value >> ((shift_count - 1) * 8));
        hex_string.append(value_to_hex_map(value));
    }

    return hex_string;
}

template <uint8_t>
std::string value_to_hex(uint8_t _value)
{
    return value_to_hex_map(_value);
}

template <typename IterArray>
std::string array_sub_to_hex(const IterArray & _array,
                             std::size_t _pos, std::size_t _size,
                             const std::string & _split = "")
{
    if (_pos >= _array.size()) {
        THROW(std::invalid_argument("_pos >= _array.size()"));
    }

    std::size_t up_limit = _pos + _size;
    up_limit = up_limit < _pos ?
                   _array.size() : std::min(up_limit, _array.size());

    std::string hex_string;
    hex_string.append(value_to_hex_map(static_cast<uint8_t>(_array.at(_pos))));
    for (++_pos; _pos < up_limit; ++_pos) {
        hex_string.append(_split);
        hex_string.append(value_to_hex_map(static_cast<uint8_t>(_array.at(_pos))));
    }

    return hex_string;
}

template <typename IterArray>
std::string array_to_hex(const IterArray & _array, const std::string & _split = "")
{
    return array_sub_to_hex(_array, 0, std::string::npos, _split);
}

template <typename Pointer>
std::string array_to_hex(const Pointer * _data, std::size_t _size, const std::string & _split = "")
{
    if (_data == nullptr || _size == 0) {
        THROW(std::invalid_argument("_data == nullptr || _size == 0"));
    }

    const uint8_t * ptr = reinterpret_cast<const uint8_t *>(_data);

    std::string hex_string;
    hex_string.append(value_to_hex_map(static_cast<uint8_t>(*ptr)));
    for (++ptr; ptr < _data + _size; ++ptr) {
        hex_string.append(_split);
        hex_string.append(value_to_hex_map(static_cast<uint8_t>(*ptr)));
    }

    return hex_string;
}

} // namespace oxygen
} // hsae
