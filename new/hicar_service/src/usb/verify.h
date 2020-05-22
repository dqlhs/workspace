#pragma once

#include <array>

#include <openssl/sha.h>

namespace hsae {
std::array<uint8_t, SHA256_DIGEST_LENGTH> sha256(const uint8_t * _data, size_t _size);

template <typename _T>
std::array<uint8_t, SHA256_DIGEST_LENGTH> sha256(const _T & _val)
{
    return sha256(reinterpret_cast<const uint8_t *>(_val.data()), _val.size());
}

} // namespace hsae
