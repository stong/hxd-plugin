#include "DataInspectorPluginServer.h"
#include "StringLitConverter.h"
#include "ArrayConverter.h"
#include "Base64Converter.h"
#include "Evil.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (SetupEvil())
        {
            MessageBoxA(NULL, "This version of HxD is not supported by data conversion plugin. Limiting the selection size to 0x1000.", "Error", MB_OK | MB_ICONERROR);
        }

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
