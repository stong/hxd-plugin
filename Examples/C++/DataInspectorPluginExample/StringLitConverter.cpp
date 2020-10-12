#include <stdlib.h>
#include <sstream>

#include "StringLitConverter.h"


TExternalDataTypeConverter* StringLitConverter::Create()
{
    return new StringLitConverter();
}

StringLitConverter::StringLitConverter()
{
    FTypeName = L"String literal";
    FFriendlyTypeName = FTypeName;
    FWidth = dtwVariable;
    FMaxTypeSize = INT_MAX;
    FSupportedByteOrders = 1 << boLittleEndian | 1 << boBigEndian;
}

void StringLitConverter::ChangeByteOrder(uint8_t* Bytes, int ByteCount,
    TByteOrder TargetByteOrder)
{
    // idk
}

TBytesToStrError StringLitConverter::BytesToStr(uint8_t* Bytes, int ByteCount,
    TIntegerDisplayOption IntegerDisplayOption, int& ConvertedByteCount,
    std::wstring& ConvertedStr)
{
    if (ByteCount < 1)
    {
        return btseBytesTooShort;
    }
    std::wostringstream ss;
    ss << L"\"";
    for (int i = 0; i < ByteCount; i++)
    {
        ss << L"\\x";
        ss << std::hex << Bytes[i];
    }
    ss << L"\"";
    ConvertedStr = ss.str();
    return btseNone;
}

TStrToBytesError StringLitConverter::StrToBytes(std::wstring Str,
    TIntegerDisplayOption IntegerDisplayOption,
    std::vector<uint8_t>& ConvertedBytes)
{
    return stbeNone;
}
