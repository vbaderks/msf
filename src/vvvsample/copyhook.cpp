//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "resource.h"


// This sample will watch folder delete requests.
// When the name includes the substring 'VVV' it will display an conformation dialogbox.
// Note: explorer.exe only read at startup the CopyHook extensions from the registry.

class __declspec(novtable) __declspec(uuid("B7096869-8E27-4f13-A9B9-3164F6D30BAB")) CopyHook :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<CopyHook, &__uuidof(CopyHook)>,
    public msf::CopyHookImpl<CopyHook>
{
public:
    BEGIN_COM_MAP(CopyHook)
        COM_INTERFACE_ENTRY(ICopyHook)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL registerInRegistry) noexcept
    {
        return msf::CopyHookImpl<CopyHook>::UpdateRegistry(registerInRegistry, IDR_COPYHOOK,
            L"VVV Sample CopyHook ShellExtension", L"VVV CopyHook");
    }

    // ICopyHook overrides
    UINT __stdcall CopyCallback(_In_opt_ HWND hwnd, UINT wFunc, UINT /*wFlags*/, _In_ LPCTSTR pszSrcFile, DWORD /*dwSrcAttribs*/,
                                   _In_opt_ LPCTSTR /*pszDestFile*/, DWORD /*dwDestAttribs*/) noexcept final
    {
        if (wFunc == FO_DELETE && ATL::CString(pszSrcFile).Find(L"VVV") != -1)
        {
            return IsolationAwareMessageBox(hwnd, msf::LoadResourceString(IDS_COPYHOOK_QUESTION).c_str(),
                msf::LoadResourceString(IDS_COPYHOOK_CAPTION).c_str(), MB_YESNOCANCEL);
        }

        return IDYES;
    }
};


OBJECT_ENTRY_AUTO(__uuidof(CopyHook), CopyHook)
