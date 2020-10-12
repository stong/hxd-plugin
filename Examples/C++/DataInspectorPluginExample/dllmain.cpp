#include "DataInspectorPluginServer.h"
#include "StringLitConverter.h"
#include "ArrayConverter.h"
#include "Base64Converter.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        RegisterDataTypeConverter(StringLitConverter::Create);
        RegisterDataTypeConverter(ArrayConverter::Create);
        RegisterDataTypeConverter(Base64Converter::Create);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
