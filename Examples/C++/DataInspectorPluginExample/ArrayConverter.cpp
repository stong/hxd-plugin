#include <stdlib.h>
#include <sstream>

#include "ArrayConverter.h"


TExternalDataTypeConverter* ArrayConverter::Create()
{
    return new ArrayConverter();
}

ArrayConverter::ArrayConverter()
{
    FTypeName = L"Python Array";
    FFriendlyTypeName = FTypeName;
    FWidth = dtwVariable;
    FMaxTypeSize = INT_MAX;
    FSupportedByteOrders = 1 << boLittleEndian | 1 << boBigEndian;
}

void ArrayConverter::ChangeByteOrder(uint8_t* Bytes, int ByteCount,
    TByteOrder TargetByteOrder)
{
    // idk
}

TBytesToStrError ArrayConverter::BytesToStr(uint8_t* Bytes, int ByteCount,
    TIntegerDisplayOption IntegerDisplayOption, int& ConvertedByteCount,
    std::wstring& ConvertedStr)
{
    if (ByteCount < 1)
    {
        return btseBytesTooShort;
    }
    std::wostringstream ss;
    ss << L"[";
    for (int i = 0; i < ByteCount; i++)
    {
        ss << L"0x";
        ss << std::hex << Bytes[i];
        if (i < ByteCount - 1)
        {
            ss << L", ";
        }
    }
    ss << L"]";
    ConvertedStr = ss.str();
    return btseNone;
}

TStrToBytesError ArrayConverter::StrToBytes(std::wstring Str,
    TIntegerDisplayOption IntegerDisplayOption,
    std::vector<uint8_t>& ConvertedBytes)
{
    return stbeNone;
}
