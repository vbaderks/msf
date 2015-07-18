//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "module.h"
#include "resource.h"

#include "../include/util.h"
#include "../include/updateregistry.h"

CModule _Module;

// Purpose: DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (!_Module.DllMain(dwReason, lpReserved))
        return false;

    // Enable/Disable dynamic isolation aware code (Windows XP and up theme support)
    IsolationAwareDllMain(dwReason);

#ifdef DEBUG
    // Increase the default level to 4 to see all trace messages.
#if _ATL_VER < 0x0C00
    atlTraceCOM.SetLevel(4);
#else
    CTrace::SetLevel(4);
#endif
#endif

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        // Optimalization: dont't notify when a thread is created.
        ATLVERIFY(DisableThreadLibraryCalls(hInstance));
    }

    return true;
}

// Purpose: Used to determine whether the DLL can be unloaded by COM
STDAPI DllCanUnloadNow()
{
    HRESULT hr = _Module.DllCanUnloadNow();
    ATLTRACE2(atlTraceCOM, 0, L"SampleShellExtension::DllCanUnloadNow hr = %d (0 = S_OK -> unload ok)\n", hr);
    return hr;
}

// Purpose: Returns a class factory to create an object of the requested type
#if _MSC_VER < 1700
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
#else
_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
#endif
{
    return _Module.DllGetClassObject(rclsid, riid, ppv);
}

// Purpose: Adds entries to the system registry
STDAPI DllRegisterServer()
{
    HRESULT hr = _Module.DllRegisterServer(/* bRegTypeLib = */ false);
    if (FAILED(hr))
        return hr;

    hr = UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, true, wszVVVExtension, wszVVVFileRootExt);
    if (FAILED(hr))
        return hr;

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}

// Purpose: Removes entries from the system registry
STDAPI DllUnregisterServer()
{
    _Module.DllUnregisterServer(); // note: will fail if already unregistered; not an issue.

    UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, false, wszVVVExtension, wszVVVFileRootExt);

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}
