//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "resource.h"


// ATL COM DLLs require a single "module" instance.
class Module : public ATL::CAtlDllModuleT<Module>
{
};
Module _Module;


// Purpose: DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (!_Module.DllMain(dwReason, lpReserved))
        return false;

    // Enable/Disable dynamic isolation aware code (Windows XP and up theme support)
    MSF::IsolationAwareDllMain(dwReason);

#ifdef DEBUG
    // Increase the default level to 4 to see all trace messages.
    ATL::CTrace::SetLevel(4);
#endif

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Optimization: don't notify when a thread is created.
        ATLVERIFY(DisableThreadLibraryCalls(hInstance));
    }

    return true;
}

// Purpose: Used to determine whether the DLL can be unloaded by COM
__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow()
{
    auto hr = _Module.DllCanUnloadNow();
    ATLTRACE2(ATL::atlTraceCOM, 0, L"SampleShellExtension::DllCanUnloadNow hr = %d (0 = S_OK -> unload OK)\n", hr);
    return hr;
}

// Purpose: Returns a class factory to create an object of the requested type
_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
    return _Module.DllGetClassObject(rclsid, riid, ppv);
}

// Purpose: Adds entries to the system registry
__control_entrypoint(DllExport)
STDAPI DllRegisterServer()
{
    auto hr = _Module.DllRegisterServer(/* bRegTypeLib = */ false);
    if (FAILED(hr))
        return hr;

    hr = MSF::UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, true, wszVVVExtension, wszVVVFileRootExt);
    if (FAILED(hr))
        return hr;

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}

// Purpose: Removes entries from the system registry
__control_entrypoint(DllExport)
STDAPI DllUnregisterServer()
{
    _Module.DllUnregisterServer(); // note: will fail if already unregistered; not an issue.

    MSF::UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, false, wszVVVExtension, wszVVVFileRootExt);

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}
