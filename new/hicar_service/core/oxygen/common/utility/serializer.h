#pragma once

#include <array>
#include <vector>

#include "datatype.h"

namespace hsae {
namespace oxygen {

class serializable;

class  serializer {
public:
    serializer(std::size_t _buffer_shrink_threshold = 512);
    virtual ~serializer();

    bool serialize(const serializable *_from);

    bool serialize(const uint8_t _value);
    bool serialize(const uint16_t _value);
    bool serialize(const uint32_t _value, bool _omit_last_byte = false);
    bool serialize(const uint8_t *_data, std::size_t _length);
    bool serialize(const std::vector<byte_t> &_data);

    virtual const uint8_t * get_data() const;
    virtual uint32_t get_capacity() const;
    virtual uint32_t get_size() const;

    virtual void set_data(const uint8_t *_data, std::size_t _capacity);

    virtual void reset();

private:
    std::vector<byte_t> data_;
    std::size_t shrink_count_;
    std::size_t buffer_shrink_threshold_;
};

template <typename _T>
bool serialze_var(serializer * _to, _T && _val)
{
    return _to && _to->serialize(_val);
}

template <typename = std::string>
bool serialze_var(serializer * _to, const std::string & _val)
{
    return _to && _to->serialize(reinterpret_cast<const uint8_t *>(_val.data()), _val.size());
}

template <typename = std::vector<uint8_t>>
bool serialze_var(serializer * _to, const std::vector<uint8_t> & _val)
{
    return _to && _to->serialize(_val);
}

template <std::size_t _T, typename = std::vector<std::array<uint8_t, _T>>>
bool serialze_var(serializer * _to, const std::vector<std::array<uint8_t, _T>> & _val)
{
    if (!_to) return false;

    for (auto & ele : _val) {
        if(!_to->serialize(ele.data(), ele.size())) {
            return false;
        }
    }

    return true;
}

template <typename _T, typename ..._Type>
bool serialze_var(serializer * _to, _T && _val, _Type && ... args)
{
    return (_to && _to->serialize(_val)) ? serialze_var(_to, args...) : false;
}

} // namespace oxygen
} // namespace hsae
