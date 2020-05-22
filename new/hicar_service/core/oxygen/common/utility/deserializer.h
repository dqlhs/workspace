#pragma once

#include <array>
#include <vector>

#include "datatype.h"

namespace hsae {
namespace oxygen {

class deserializer {
public:
    deserializer(std::size_t _buffer_shrink_threshold = 512);
    deserializer(const byte_t *_data, std::size_t _length,
                                std::size_t _buffer_shrink_threshold = 512);
    deserializer(const deserializer& _other);
    virtual ~deserializer();

    void set_data(const byte_t *_data, std::size_t _length);
    void append_data(const byte_t *_data, std::size_t _length);
    void drop_data(std::size_t _length);

    std::size_t get_available() const;
    std::size_t get_remaining() const;
    void set_remaining(std::size_t _length);

    // to be used (internally) by objects to deserialize their members
    // Note: this needs to be encapsulated!
    bool deserialize(uint8_t& _value);
    bool deserialize(uint16_t& _value);
    bool deserialize(uint32_t& _value,
            bool _omit_last_byte = false);
    bool deserialize(uint8_t *_data, std::size_t _length);
    bool deserialize(std::vector<uint8_t>& _value);

    bool look_ahead(std::size_t _index, uint8_t &_value) const;
    bool look_ahead(std::size_t _index, uint16_t &_value) const;
    bool look_ahead(std::size_t _index, uint32_t &_value) const;

    void reset();

#ifdef OXYGEN_DEBUGGING
    void show() const;
#endif
protected:
    std::vector<byte_t> data_;
    std::vector<byte_t>::iterator position_;
    std::size_t remaining_;
private:
    const std::size_t buffer_shrink_threshold_;
    std::size_t shrink_count_;

};

template <typename _T>
bool deserialize_var(deserializer * _from, _T && _val)
{
    return _from && _from->deserialize(_val);
}

template <typename = std::string>
bool deserialize_var(deserializer * _from, std::string & _val)
{
    _val.resize(_from->get_remaining());
    return _from && _from->deserialize(reinterpret_cast<uint8_t *>(&_val.front()), _val.size());
}

template <typename = std::vector<uint8_t>>
bool deserialize_var(deserializer * _from, std::vector<uint8_t> & _val)
{
    _val.resize(_from->get_remaining());
    return _from && _from->deserialize(_val);
}

template <std::size_t _T, typename = std::vector<std::array<uint8_t, _T>>>
bool deserialize_var(deserializer * _from, std::vector<std::array<uint8_t, _T>> & _val)
{
    if (!_from) return false;

    auto remaining_size = _from->get_remaining() / _T;
    for (;remaining_size != 0; remaining_size--) {
        std::array<uint8_t, _T> array_value;
        if (!_from->deserialize(array_value.data(), array_value.size())) {
            return false;
        }

        _val.push_back(std::move(array_value));
    }

    return true;
}

template <typename _T, typename ..._Type>
bool deserialize_var(deserializer * _from, _T && _val, _Type && ... args)
{
    return (_from && _from->deserialize(_val)) ? deserialize_var(_from, args...) : false;
}

template <typename _T, typename _D, typename = typename std::enable_if<std::is_class<_T>::value>::type>
bool deserialize_data(_D && _data, _T & _v) noexcept
{
    deserializer des((const uint8_t *)_data.data(), _data.size());
    return _v.deserialize(&des);
}

template <typename _T, typename _D, typename _Size,
          typename = typename std::enable_if<std::is_integral<_Size>::value>::type>
bool deserialize_data(const _D * _data, _Size _size, _T & _v) noexcept
{
    deserializer des((const uint8_t *)_data, _size);
    return _v.deserialize(&des);
}

template <typename _T, typename _D>
_T deserialize_data(_D && _data)
{
    _T value;
    deserializer des((const uint8_t *)_data.data(), _data.size());
    if(!value.deserialize(&des)) {
        std::runtime_error("deserialize error");
    }
    return std::move(value);
}

template <typename _T, typename _D, typename _Size,
          typename = typename std::enable_if<std::is_integral<_Size>::value>::type>
_T deserialize_data(const _D * _data, _Size _size)
{
    _T value;
    deserializer des((const uint8_t *)_data, _size);
    if(!value.deserialize(&des)) {
        std::runtime_error("deserialize error");
    }

    return std::move(value);
}

} // namespace oxygen
} // namespace hsae
