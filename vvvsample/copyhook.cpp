//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/copyhookimpl.h"
#include "../include/strutil.h"
#include "copyhookclsid.h"
#include "resource.h"

// This sample will watch folder delete requests. 
// When the name includes the substring 'VVV' it will display an conformation dialogbox.
// Note: explorer.exe only read at startup the CopyHook extensions from the registry.

class ATL_NO_VTABLE CCopyHook :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCopyHook, &__uuidof(CCopyHook)>,
    public ICopyHookImpl<CCopyHook>
{
public:
    BEGIN_COM_MAP(CCopyHook)
        COM_INTERFACE_ENTRY(ICopyHook)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
    {
        return ICopyHookImpl<CCopyHook>::UpdateRegistry(bRegister, IDR_COPYHOOK,
            L"VVV Sample CopyHook ShellExtension", L"VVV CopyHook");
    }


    // ICopyHook overrides
    STDMETHOD_(UINT, CopyCallback)(_In_opt_ HWND hwnd, UINT wFunc, UINT /*wFlags*/, _In_ LPCTSTR pszSrcFile, DWORD /*dwSrcAttribs*/,
                                   _In_opt_ LPCTSTR /*pszDestFile*/, DWORD /*dwDestAttribs*/)
    {
        if (wFunc == FO_DELETE && CString(pszSrcFile).Find(_T("VVV")) != -1)
        {
            return IsolationAwareMessageBox(hwnd, LoadString(IDS_COPYHOOK_QUESTION), 
                LoadString(IDS_COPYHOOK_CAPTION), MB_YESNOCANCEL);
        }
        else
        {
            return IDYES;
        }
    }
};


OBJECT_ENTRY_AUTO(__uuidof(CCopyHook), CCopyHook)
