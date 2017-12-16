//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "vvvfile.h"
#include "resource.h"

using std::wstring;

class __declspec(novtable) __declspec(uuid("EDD37CEF-F1E0-42bb-9AEF-177E0306AA71")) InfoTip :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<InfoTip, &__uuidof(InfoTip)>,
    public msf::InfoTipImpl<InfoTip>
{
public:
    BEGIN_COM_MAP(InfoTip)
        COM_INTERFACE_ENTRY(IQueryInfo)
        COM_INTERFACE_ENTRY(IInitializeWithFile)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL registerInRegistry) noexcept
    {
        return InfoTipImpl<InfoTip>::UpdateRegistry(registerInRegistry, IDR_INFOTIP,
            L"VVV Sample InfoTip ShellExtension", wszVVVFileRootExt);
    }

    void InitializeCore(const wchar_t* filename, DWORD /*dwMode*/) override
    {
        VVVFile vvvfile{ filename };

        m_label = vvvfile.GetLabel();
        m_fileCount = msf::ToString(vvvfile.GetFileCount());
    }

    // Purpose: called by the shell/msf when it needs the text for the infotip.
    //          The string is used for the tooltip and the text in the status bar.
    wstring GetInfoTip(DWORD /* dwFlags */) override
    {
        return msf::LoadResourceString(IDS_SHELLEXT_LABEL) + L": " + m_label + L"\n" +
               msf::LoadResourceString(IDS_SHELLEXT_FILECOUNT) + L": " + m_fileCount;
    }

private:
    wstring m_label;
    wstring m_fileCount;
};

OBJECT_ENTRY_AUTO(__uuidof(InfoTip), InfoTip)
