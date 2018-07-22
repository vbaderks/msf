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

    static HRESULT __stdcall UpdateRegistry(BOOL registerInRegistry) noexcept
    {
        return InfoTipImpl<InfoTip>::UpdateRegistry(registerInRegistry, IDR_INFOTIP,
            L"VVV Sample InfoTip ShellExtension", wszVVVFileRootExt);
    }

    InfoTip(const InfoTip&) = delete;
    InfoTip(InfoTip&&) = delete;
    InfoTip& operator=(const InfoTip&) = delete;
    InfoTip& operator=(InfoTip&&) = delete;

    void InitializeCore(PCWSTR filename, DWORD /*dwMode*/) final
    {
        VVVFile vvvfile{ filename };

        m_inftoTip =
            msf::LoadResourceString(IDS_SHELLEXT_LABEL) + L": " + vvvfile.GetLabel() + L"\n" +
            msf::LoadResourceString(IDS_SHELLEXT_FILECOUNT) + L": " + std::to_wstring(vvvfile.GetFileCount());
    }

    // Purpose: called by the shell/msf when it needs the text for the info tip.
    //          The string is used for the tooltip and the text in the status bar.
    PCWSTR GetInfoTip(DWORD /* dwFlags */) final
    {
        return m_inftoTip.c_str();
    }

protected:
    InfoTip() = default;
    ~InfoTip() = default;

private:
    wstring m_inftoTip;
};

OBJECT_ENTRY_AUTO(__uuidof(InfoTip), InfoTip)
