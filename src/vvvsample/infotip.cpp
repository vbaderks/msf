//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "vvvfile.h"
#include "resource.h"

#include "../include/infotipimpl.h"
#include "../include/strutil.h"

class ATL_NO_VTABLE __declspec(uuid("EDD37CEF-F1E0-42bb-9AEF-177E0306AA71")) CInfoTip :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CInfoTip, &__uuidof(CInfoTip)>,
    public InfoTipImpl<CInfoTip>
{
public:
    BEGIN_COM_MAP(CInfoTip)
        COM_INTERFACE_ENTRY(IQueryInfo)
        COM_INTERFACE_ENTRY(IInitializeWithFile)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) noexcept
    {
        return InfoTipImpl<CInfoTip>::UpdateRegistry(bRegister, IDR_INFOTIP,
            L"VVV Sample InfoTip ShellExtension", wszVVVFileRootExt);
    }

    void InitializeImpl(const wchar_t* szFilename, DWORD /*dwMode*/)
    {
        CVVVFile vvvfile(szFilename);

        _strLabel = vvvfile.GetLabel();
        _strFileCount = MSF::ToString(vvvfile.GetFileCount());
    }

    // Purpose: called by the shell/MSF when it needs the text for the infotip.
    //          The string is used for the tooltip and the text in the statusbar.
    CString GetInfoTip(DWORD /* dwFlags */) const
    {
        return LoadString(IDS_SHELLEXT_LABEL) + L": " + _strLabel + L"\n" +
               LoadString(IDS_SHELLEXT_FILECOUNT) + L": " + _strFileCount;
    }

private:
    CString _strLabel;
    CString _strFileCount;
};

OBJECT_ENTRY_AUTO(__uuidof(CInfoTip), CInfoTip)
