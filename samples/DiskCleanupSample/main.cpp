//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "pch.h"


class Module final : public ATL::CAtlDllModuleT<Module>
{
};

MSF_WARNING_SUPPRESS(26426) // Global initializer calls a non-constexpr function 'Module::Module'
Module _AtlModule;
MSF_WARNING_UNSUPPRESS()


// DLL Entry Point
extern "C" BOOL __stdcall DllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID lpReserved)
{
    return _AtlModule.DllMain(dwReason, lpReserved);
}

_Use_decl_annotations_
STDAPI DllCanUnloadNow()
{
    return _AtlModule.DllCanUnloadNow();
}

// Returns a class factory to create an object of the requested type.
_Use_decl_annotations_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

// DllRegisterServer - Adds entries to the system registry.
_Use_decl_annotations_
STDAPI DllRegisterServer()
{
    // registers object, typelib and all interfaces in typelib
    return _AtlModule.DllRegisterServer();
}

// DllUnregisterServer - Removes entries from the system registry.
_Use_decl_annotations_
STDAPI DllUnregisterServer()
{
    return _AtlModule.DllUnregisterServer();
}
