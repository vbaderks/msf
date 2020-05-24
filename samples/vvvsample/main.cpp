//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//

#include "pch.h"

#include "resource.h"


// ATL COM DLLs require a single "module" instance.
class Module : public ATL::CAtlDllModuleT<Module>
{
};

MSF_WARNING_SUPPRESS(26426) // Global initializer calls a non-constexpr function 'Module::Module'
Module _Module;
MSF_WARNING_UNSUPPRESS()

// Purpose: DLL Entry Point
extern "C" BOOL __stdcall DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (!_Module.DllMain(dwReason, lpReserved))
        return false;

    // Enable/Disable dynamic isolation aware code (Windows XP and up theme support)
    msf::IsolationAwareDllMain(dwReason);

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
#ifdef DEBUG
        // Increase the default level to 4 to see all trace messages.
        ATL::CTrace::SetLevel(4);
#endif

        // Optimization: don't notify when a thread is created.
        ATLVERIFY(DisableThreadLibraryCalls(hInstance));
        break;

    case DLL_PROCESS_DETACH:
        ATLTRACE(L"vvvsample::DllMain DLL_PROCESS_DETACH");
        break;

    default:
        // Do nothing for the rest of the cases
        break;
    }

    return true;
}

// Purpose: Used to determine whether the DLL can be unloaded by COM
__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow()
{
    const auto hr = _Module.DllCanUnloadNow();
    ATLTRACE(L"vvvsample::DllCanUnloadNow hr = %d (0 = S_OK -> unload OK)\n", hr);
    return hr;
}

// Purpose: Returns a class factory to create an object of the requested type
_Check_return_
STDAPI DllGetClassObject(_In_ REFCLSID classId, _In_ REFIID interfaceId, _Outptr_ LPVOID* ppv)
{
    return _Module.DllGetClassObject(classId, interfaceId, ppv);
}

// Purpose: Adds entries to the system registry
#if WDK_NTDDI_VERSION < 0x0A000007 // = ABRACADABRA_WIN10_19H1
__control_entrypoint(DllExport)
#endif
STDAPI DllRegisterServer()
{
    auto hr = _Module.DllRegisterServer(/* bRegTypeLib = */ false);
    if (FAILED(hr))
    {
        ATLTRACE(L"vvvsample::DllRegisterServer failed (1) hr = %d\n", hr);
        return hr;
    }

    hr = msf::UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, true, wszVVVExtension, wszVVVFileRootExt);
    if (FAILED(hr))
    {
        ATLTRACE(L"vvvsample::DllRegisterServer failed (2) hr = %d\n", hr);
        return hr;
    }

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}

// Purpose: Remove entries from the system registry
#if WDK_NTDDI_VERSION < 0x0A000007 // = ABRACADABRA_WIN10_19H1
__control_entrypoint(DllExport)
#endif
STDAPI DllUnregisterServer()
{
    _Module.DllUnregisterServer(); // note: will fail if already unregistered; not an issue.

    msf::UpdateRegistryConnectExtensionToProgId(IDR_EXTENSION, false, wszVVVExtension, wszVVVFileRootExt);

    // Notify the shell that .vvv file association has changed.
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

    return S_OK;
}
