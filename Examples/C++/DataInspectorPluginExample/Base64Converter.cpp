#include <stdlib.h>
#include <sstream>

#include "Base64Converter.h"
#include "Evil.h"

// https://stackoverflow.com/a/13935718/8624655
static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";

static inline bool is_base64(char c) {
    return isalnum(c) || (c == '+') || (c == '/');
}

static std::wstring base64_encode(uint8_t const* buf, unsigned int bufLen) {
    std::wstring ret;
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (bufLen--) {
        char_array_3[i++] = *(buf++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

static bool base64_decode(std::wstring const& encoded_string, std::vector<uint8_t>& ret) {
    size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];

    for (size_t i = 0; i < in_len; i++) {
        if (!iswascii(encoded_string[i]) || !is_base64((uint8_t)encoded_string[i])) {
            return false;
        }
    }

    while (in_len-- && (encoded_string[in_] != L'='))  {
        char_array_4[i++] = (uint8_t) encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = (uint8_t) base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = (uint8_t) base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return true;
}

TExternalDataTypeConverter* Base64Converter::Create()
{
    return new Base64Converter();
}

Base64Converter::Base64Converter()
{
    FTypeName = L"Base64";
    FFriendlyTypeName = FTypeName;
    FWidth = dtwVariable;
    FMaxTypeSize = INT_MAX;
    FSupportedByteOrders = 1 << boLittleEndian | 1 << boBigEndian;
}

void Base64Converter::ChangeByteOrder(uint8_t* Bytes, int ByteCount,
    TByteOrder TargetByteOrder)
{
    // idk
}

TBytesToStrError Base64Converter::BytesToStr(uint8_t* Bytes, int ByteCount,
    TIntegerDisplayOption IntegerDisplayOption, int& ConvertedByteCount,
    std::wstring& ConvertedStr)
{
    ByteCount = ClampDataSize(ByteCount);
    if (ByteCount < 1)
    {
        return btseBytesTooShort;
    }
    ConvertedStr = base64_encode(Bytes, ByteCount);
    return btseNone;
}

TStrToBytesError Base64Converter::StrToBytes(std::wstring Str,
    TIntegerDisplayOption IntegerDisplayOption,
    std::vector<uint8_t>& ConvertedBytes)
{
    bool valid = base64_decode(Str, ConvertedBytes);
    if (!valid)
    {
        return stbeInvalidString;
    }
    return stbeNone;
}
