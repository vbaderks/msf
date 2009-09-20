#include "stdafx.h"
#include "../include/infotipimpl.h"
#include "../include/strutil.h"
#include "shellfolderclsid.h"
#include "resource.h"

// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("F3582A81-0D4A-42db-80B6-2843BC4536AC") CInfoTip;

class ATL_NO_VTABLE CInfoTip :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CInfoTip, &__uuidof(CInfoTip)>,
    public IInfoTipImpl<CInfoTip>
{
public:
    BEGIN_COM_MAP(CInfoTip)
        COM_INTERFACE_ENTRY(IInitializeWithFile) // Used and prefered by Vista.
        COM_INTERFACE_ENTRY(IPersistFile)
        COM_INTERFACE_ENTRY(IQueryInfo)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
    {
        // Note: Add a description and an extension
        return IInfoTipImpl<CInfoTip>::UpdateRegistry(IDR_INFOTIP, bRegister,
            L"<description>", __uuidof(CShellFolder), L"<.extension>");
    }

    // Purpose: called by the shell/MSF to initialize the instance from a filename.
    void InitializeImpl(const TCHAR* szFilename, DWORD /*dwMode*/)
    {
        (szFilename); // not used (yet)
        // Note: initialize the instance.
    }

    // Purpose: called by the shell/MSF when it needs the text for the infotip.
    //          The string is used for the tooltip and the text in the statusbar.
    CString GetInfoTip(DWORD /* dwFlags */)
    {
        // Note: Implement this function to return a real tooltip text.
        return _T(""); 
    }
};

OBJECT_ENTRY_AUTO(__uuidof(CInfoTip), CInfoTip)
