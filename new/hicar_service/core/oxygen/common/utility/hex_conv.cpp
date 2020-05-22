#include "hex_conv.h"

#include "exception/throw.h"

namespace hsae {
namespace oxygen {

static const uint8_t hex_to_value_map [] {
    //  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 00
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, // 30
    0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 40
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 50
    0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 60
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 70
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 90
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // B0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // C0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // D0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // E0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // F0
};

uint8_t hex_to_uint8(char c1, char c2)
{
    return (uint8_t)((hex_to_value_map[c1 & 0xff] << 4) + hex_to_value_map[c2 & 0xff]);
}

std::vector<uint8_t> hex_to_vec(const std::string & _string, std::size_t _pos, std::size_t _size)
{
    std::size_t max = std::max(_size, _pos + _size);
    std::size_t end = std::min(_string.size(), max);
    std::vector<uint8_t> out;
    for(++_pos; _pos < end ; _pos += 2) {
        out.push_back(hex_to_uint8(_string.at(_pos - 1), _string.at(_pos)));
    }

    return out;
}

uint8_t hex_to_uint8(const std::array<char, 2> & _data)
{
    return hex_to_uint8(_data[0], _data[1]);
}

uint16_t hex_to_uint16(const std::array<char, 4> & _data)
{
    return (uint16_t)((hex_to_uint8(_data[0], _data[1]) << 8)
            + hex_to_uint8(_data[2], _data[3]));
}

uint8_t hex_2byte_to_uint8(const std::string & _string, std::size_t _pos)
{
    if (_string.size() <= _pos + 1) {
        throw std::runtime_error("_string.size() <= _pos + 1");
    }
    return hex_to_uint8(_string.at(_pos), _string.at(_pos + 1));
}

uint16_t hex_4byte_to_uint16(const std::string & _string, std::size_t _pos)
{
    return (uint16_t)((hex_2byte_to_uint8(_string, _pos) << 8) + hex_2byte_to_uint8(_string, _pos + 2));
}

uint32_t hex_6byte_to_uint32(const std::string & _string, std::size_t _pos)
{
    return (uint32_t)((hex_4byte_to_uint16(_string, _pos) << 8) + hex_2byte_to_uint8(_string, _pos + 4));
}

uint32_t hex_8byte_to_uint32(const std::string & _string, std::size_t _pos)
{
    return (uint32_t)((hex_4byte_to_uint16(_string, _pos) << 16) + hex_4byte_to_uint16(_string, _pos + 4));
}


const static char * value_to_hex_map_array [] = {
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", // 00
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", // 10
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", // 20
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", // 30
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", // 40
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F", // 50
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F", // 60
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", // 70
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F", // 80
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", // 90
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", // A0
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", // B0
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", // C0
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", // D0
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF", // E0
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF", // F0
};

std::string value_to_hex_map(uint8_t _value)
{
    return value_to_hex_map_array[_value];
}

const static char * value_to_hex_map_without_fill_zero_array [] = {
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
      "",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",  "A",  "B",  "C",  "D",  "E",  "F", // 00
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", // 10
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", // 20
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", // 30
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", // 40
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F", // 50
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F", // 60
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", // 70
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F", // 80
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", // 90
    "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", // A0
    "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", // B0
    "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", // C0
    "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", // D0
    "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF", // E0
    "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF", // F0
};

std::string value_to_hex_map_without_fill_zero(uint8_t _value)
{
    return value_to_hex_map_without_fill_zero_array[_value];
}



} // namespace oxygen
} // hsae
