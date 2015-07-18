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

    // Enable/Disable dynamic isolation aware code (Windows XP theme support)
    IsolationAwareDllMain(dwReason);

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
    ATLTRACE2(atlTraceCOM, 0, _T("SampleShellExtension::DllCanUnloadNow hr = %d\n"), hr);
    return hr;
}

// Purpose: Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.DllGetClassObject(rclsid, riid, ppv);
}

// Purpose: Adds entries to the system registry
STDAPI DllRegisterServer()
{
    HRESULT hr = _Module.DllRegisterServer(/* bRegTypeLib = */ false);
    if (FAILED(hr))
        return hr;

    // TODO: update extension and root extension.
    hr = UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, true, L"<extension>", L"<rootextension>");
    if (FAILED(hr))
        return hr;

    // Notify the shell that file associations have changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return hr;
}

// Purpose: Removes entries from the system registry
STDAPI DllUnregisterServer()
{
    _Module.DllUnregisterServer(); // note: will fail if already unregistered; not an issue.

    // Notify the shell that file associations have changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    return S_OK;
}
