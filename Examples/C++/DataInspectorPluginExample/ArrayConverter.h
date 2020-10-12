#pragma once

#include "DataInspectorShared.h"
#include "DataInspectorPluginServer.h"

class ArrayConverter : public TExternalDataTypeConverter
{
public:
    // Class factory function / virtual constructor
    static TExternalDataTypeConverter* Create();
public:
    ArrayConverter();

    void ChangeByteOrder(uint8_t* Bytes, int ByteCount,
        TByteOrder TargetByteOrder) override;
    TBytesToStrError BytesToStr(uint8_t* Bytes, int ByteCount,
        TIntegerDisplayOption IntegerDisplayOption, int& ConvertedByteCount,
        std::wstring& ConvertedStr) override;
    TStrToBytesError StrToBytes(std::wstring Str,
        TIntegerDisplayOption IntegerDisplayOption,
        std::vector<uint8_t>& ConvertedBytes) override;
};
