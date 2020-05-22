#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace hsae {

typedef std::vector<uint8_t> hicar_usb_buf;

template <typename Base, typename Append>
void data_append(Base & _base, Append & _append)
{
    std::copy(_append.begin(), _append.end(), std::back_inserter(_base));
}


hicar_usb_buf ea_format(uint32_t _value);
void ea_format(uint32_t _value, hicar_usb_buf & _data);

bool ea_format_parse(const uint8_t * & _begin, const uint8_t * _end, uint32_t & _value);

struct tlv {
    uint32_t type;
    hicar_usb_buf data;

    tlv();
    tlv(uint32_t _type);
    tlv(uint32_t _type, const std::string & _data);
    tlv(uint32_t _type, std::vector<uint8_t> && _data);
    tlv(uint32_t _type, const std::vector<uint8_t> & _data);
};

hicar_usb_buf tlv_with_ea_format_serialize(const tlv & _tlv);
void tlv_with_ea_format_serialize(const tlv & _tlv, hicar_usb_buf & _data);

struct tlv_with_ea_format : public tlv {
    hicar_usb_buf serialize() const;
    void serialize(hicar_usb_buf & _out_data) const;
};

typedef std::vector<tlv_with_ea_format> tlv_box_with_ea_format;
typedef std::vector<tlv> tlv_box;

bool tlv_with_ea_format_parse(const uint8_t * & _begin, const uint8_t * _end, tlv & _tlv);
bool tlv_with_ea_format_parse(const uint8_t * & _begin, const uint8_t * _end, tlv_box & _tlv_box);


hicar_usb_buf hicar_usb_business_protocol(uint8_t _type, tlv _flag, const hicar_usb_buf & _in_data);
void hicar_usb_business_protocol(uint8_t _type, tlv _flag, const hicar_usb_buf & _in_data, hicar_usb_buf & _out_data);

bool hicar_usb_business_protocol_parse(const uint8_t * _begin, const uint8_t * _end,
                                       uint32_t & _type, tlv & _flag, hicar_usb_buf & _data);


} // namespace hsae
