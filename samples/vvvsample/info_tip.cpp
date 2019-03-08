//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//

#include "pch.h"

#include "vvv_file.h"
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
        VVVFile vvvFile{ filename };

        m_infoTip =
            msf::LoadResourceString(IDS_SHELLEXT_LABEL) + L": " + vvvFile.GetLabel() + L"\n" +
            msf::LoadResourceString(IDS_SHELLEXT_FILECOUNT) + L": " + std::to_wstring(vvvFile.GetFileCount());
    }

    // Purpose: called by the shell/msf when it needs the text for the info tip.
    //          The string is used for the tooltip and the text in the status bar.
    PCWSTR GetInfoTip(DWORD /* dwFlags */) noexcept(false) final
    {
        return m_infoTip.c_str();
    }

protected:
    InfoTip() noexcept(false) = default; // noexcept(false) needed as ATL base class is not defined noexcept.
    ~InfoTip() = default;

private:
    wstring m_infoTip;
};

OBJECT_ENTRY_AUTO(__uuidof(InfoTip), InfoTip)
