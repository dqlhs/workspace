#include "verify.h"

#include "exception/throw.h"

namespace hsae {

std::array<uint8_t, SHA256_DIGEST_LENGTH> sha256(const uint8_t * _data, size_t _size)
{
    std::array<uint8_t, SHA256_DIGEST_LENGTH> hash;
    SHA256_CTX sha256;
    bool is_successful = (SHA256_Init(&sha256) == 1)
                         && (SHA256_Update(&sha256, _data, _size) == 1)
                         && (SHA256_Final(hash.data(), &sha256) == 1);

    THROW_EXP(is_successful, std::runtime_error("errir"));

    return hash;
}

} // namespace hsae
