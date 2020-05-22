#include "hicar_usb_protocol.h"

#include <algorithm>

namespace hsae {

const static uint16_t CRC_TABLE[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};

static uint16_t crc16_create(const uint8_t * _bytes, size_t len) {
    uint16_t crc = 0x0000;

    for (size_t i = 0; i < len; ++i) {
        uint8_t b = _bytes[i];
        crc = (crc >> 8) ^ CRC_TABLE[(crc ^ b) & 0xff];
    }
    return crc;
}

hicar_usb_buf tlv_with_ea_format::serialize() const
{
    return tlv_with_ea_format_serialize(*this);
}

void tlv_with_ea_format::serialize(hicar_usb_buf & _out_data) const
{
    tlv_with_ea_format_serialize(*this, _out_data);
}

hicar_usb_buf ea_format(uint32_t _value)
{
    hicar_usb_buf vec;
    ea_format(_value,vec);
    std::reverse(vec.begin(), vec.end());
    return vec;
}


void ea_format(uint32_t _value, hicar_usb_buf & _data)
{
    std::vector<uint8_t> ea_data;
    size_t shift_max_count = ((sizeof (_value) * 8) / 7)
                             + ((sizeof (_value) * 8) % 7 != 0 ? 1 : 0);

    for (size_t index = 0; index < shift_max_count; ++index) {
        ea_data.push_back((_value >> (index * 7)) & 0x7f);
    }

    for (size_t index = shift_max_count; index != 0; --index) {
        uint8_t & data = ea_data[index - 1];
        if (data != 0x00
                || index == 1) {
            data |= 0x80;
            std::copy(ea_data.begin(), ea_data.begin() + index, std::back_inserter(_data));
            return;
        }
    }
}

tlv::tlv() : type(0)
{

}

tlv::tlv(uint32_t _type) : type (_type){ }

tlv::tlv(uint32_t _type, const std::string & _data) : type(_type)
{
    data_append(data, _data);
}

tlv::tlv(uint32_t _type, std::vector<uint8_t> && _data) : type(_type), data(std::move(_data))
{
    
}

tlv::tlv(uint32_t _type, const std::vector<uint8_t> & _data) : type(_type), data(_data)
{
    
}

bool tlv_with_ea_format_parse(const uint8_t * & _begin, const uint8_t * _end, tlv & _tlv)
{
    tlv tlv_ele;
    uint32_t value = 0;
    bool is_successful = (ea_format_parse(_begin, _end, tlv_ele.type))
                         // get tlv length
                         && (ea_format_parse(_begin, _end, value))
                         // check _data remain length >= tlv length
                         && (static_cast<size_t>(_end - _begin) >= value);

    if (is_successful) {
        // copy tlv value
        std::copy(_begin, _begin + value, std::back_inserter(tlv_ele.data));
        _begin += value;
        _tlv = std::move(tlv_ele);
    }

    return is_successful;
}

bool tlv_with_ea_format_parse(const uint8_t * & _begin, const uint8_t * _end, tlv_box & _tlv_box)
{
    tlv_box vec_tlv;
    tlv tlv_ele;
    bool is_successful = true;
    while (is_successful && _begin != _end) {
        is_successful = tlv_with_ea_format_parse(_begin, _end, tlv_ele);
        if (is_successful) {
            vec_tlv.emplace_back(std::move(tlv_ele));
        }
    }

    if (is_successful) {
        _tlv_box.swap(vec_tlv);
    }

    return is_successful;
}

bool ea_format_parse(const uint8_t *& _begin, const uint8_t * _end, uint32_t & _value)
{
    uint32_t value = 0;

    size_t min = std::min<size_t>(sizeof (_value ),
                                  static_cast<size_t>(_end - _begin));
    for (size_t index = 0; index < min; ++index, ++_begin) {
        uint8_t ele_value = *_begin;
        value |= (static_cast<uint32_t>(ele_value & 0x7f) << (7 * 0));
        if ((ele_value & 0x80) == 0x80) {
            _value = value;
            ++_begin;
            return true;
        }
    }

    return false;
}

hicar_usb_buf hicar_usb_business_protocol(uint8_t _type, tlv _flag,
                                                 const hicar_usb_buf & _in_data)
{
    hicar_usb_buf protocol;
    hicar_usb_business_protocol(_type, _flag, _in_data, protocol);

    return protocol;
}


void hicar_usb_business_protocol(uint8_t _type, tlv _flag,
                                 const hicar_usb_buf & _in_data, hicar_usb_buf & _out_data)
{
    // sof
    size_t size = _out_data.size();
    _out_data.push_back(0x7F);
    // type
    ea_format(_type, _out_data);
    // flag
    tlv_with_ea_format_serialize(_flag, _out_data);
    // length
    ea_format(static_cast<uint32_t>(_in_data.size()), _out_data);
    data_append(_out_data, _in_data);

    // push crc
    uint16_t crc = crc16_create(_out_data.data() + size, _out_data.size() - size);
    _out_data.push_back(crc & 0xff);
    _out_data.push_back((crc >> 8) & 0xff);
}

hicar_usb_buf tlv_with_ea_format_serialize(const tlv & _tlv)
{
    auto type_format = ea_format(_tlv.type);
    auto data_length_format = ea_format(static_cast<uint32_t>(_tlv.data.size()));

    decltype (type_format) ret;
    std::copy(type_format.begin(), type_format.end(), std::back_inserter(ret));
    std::copy(data_length_format.begin(), data_length_format.end(), std::back_inserter(ret));
    std::copy(_tlv.data.begin(), _tlv.data.end(), std::back_inserter(ret));

    return ret;
}

void tlv_with_ea_format_serialize(const tlv & _tlv, hicar_usb_buf & _data)
{
    ea_format(_tlv.type, _data);
    ea_format(static_cast<uint32_t>(_tlv.data.size()), _data);
    std::copy(_tlv.data.begin(), _tlv.data.end(), std::back_inserter(_data));
}

bool hicar_usb_business_protocol_parse(const uint8_t * _begin, const uint8_t * _end,
                                       uint32_t & _type, tlv & _flag, hicar_usb_buf & _data)
{
    if (_end < _begin || _end - _begin < 7) return false;
    // check crc
    uint16_t crc_data = static_cast<uint16_t>((*(_end - 1) & 0xff) << 8) | (*(_end - 2) & 0xff);
    // remove crc
    _end = _end - 2;
    uint16_t crc_value = crc16_create(_begin, static_cast<size_t>(_end - _begin));
    if (crc_data != crc_value) {
        return false;
    }

    tlv tlv_ele;
    uint32_t length = 0;
    bool is_successful = *_begin == 0x7f // check sof
                         // get type
                         && ea_format_parse(++_begin, _end, _type)
                         // get flag
                         && tlv_with_ea_format_parse(_begin, _end, _flag)
                         // get length
                         && ea_format_parse(_begin, _end, length)
                         && (length == (_end - _begin));

    if (is_successful) {
        // copy data
        std::copy(_begin, _end, std::back_inserter(_data));
    }

    return is_successful;
}





} // namespace hase
