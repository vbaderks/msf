//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/infotipimpl.h"
#include "../include/strutil.h"
#include "infotipclsid.h"
#include "shellfolderclsid.h"
#include "vvvfile.h"
#include "resource.h"


class ATL_NO_VTABLE CInfoTip :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CInfoTip, &__uuidof(CInfoTip)>,
    public IInfoTipImpl<CInfoTip>
{
public:
    BEGIN_COM_MAP(CInfoTip)
        COM_INTERFACE_ENTRY(IQueryInfo)
        COM_INTERFACE_ENTRY(IInitializeWithFile) // Used and prefered by Vista and up.
        COM_INTERFACE_ENTRY(IPersistFile)        // Used by XP and older.
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
    {
        return IInfoTipImpl<CInfoTip>::UpdateRegistry(bRegister, IDR_INFOTIP,
            L"VVV Sample ShellExtension", wszVVVFileRootExt);
    }


    void InitializeImpl(const TCHAR* szFilename, DWORD /*dwMode*/)
    {
        CVVVFile vvvfile(szFilename);

        m_strLabel = vvvfile.GetLabel();
        m_strFileCount = MSF::ToString(vvvfile.GetFileCount());
    }


    // Purpose: called by the shell/MSF when it needs the text for the infotip.
    //          The string is used for the tooltip and the text in the statusbar.
    CString GetInfoTip(DWORD /* dwFlags */)
    {
        return LoadString(IDS_SHELLEXT_LABEL) + _T(": ") + m_strLabel + _T("\n") +
               LoadString(IDS_SHELLEXT_FILECOUNT) + _T(": ") + m_strFileCount;
    }

private:
    CString m_strLabel;
    CString m_strFileCount;
};


OBJECT_ENTRY_AUTO(__uuidof(CInfoTip), CInfoTip)
