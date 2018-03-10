//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

template <typename T>
class __declspec(novtable) ShellExtPropertyPageImpl : public ATL::CSnapInPropertyPageImpl<T>
{
public:
    static UINT CALLBACK PropPageCallback(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
    {
        ShellExtPropertyPageImpl<T>* pT = reinterpret_cast<T*>(ppsp->lParam);

        switch (uMsg)
        {
        case PSPCB_ADDREF:
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellExtPropertyPageImpl::Callback (instance=%p, uMsg=AddRef)\n", pT);
            ++(pT->m_nRef); // only 5.80 (IE5) and up sends 'addref'.
            break;

        case PSPCB_CREATE:
            return ATL::CSnapInPropertyPageImpl<T>::PropPageCallback(hWnd, uMsg, ppsp);

        case PSPCB_RELEASE:
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellExtPropertyPageImpl::Callback (instance=%p, uMsg=Release)\n", pT);
            --(pT->m_nRef);
            if (pT->m_nRef <= 0)
                delete pT;
            break;

        default:
            ATLASSERT(!"Detected unexpected PSPCB uMsg");
            break;
        }

        return 1;
    }

    explicit ShellExtPropertyPageImpl(LPCTSTR lpszTitle = nullptr) noexcept :
        ATL::CSnapInPropertyPageImpl<T>(lpszTitle)
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellExtPropertyPageImpl::ShellExtPropertyPageImpl (instance=%p)\n", this);
        ATL::_pAtlModule->Lock(); // property page is not a COM object, but DLL must stay in memory.
    }

    ~ShellExtPropertyPageImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellExtPropertyPageImpl::~ShellExtPropertyPageImpl (instance=%p)\n", this);
        ATL::_pAtlModule->Unlock();
    }

    ShellExtPropertyPageImpl(const ShellExtPropertyPageImpl&) = delete;
    ShellExtPropertyPageImpl(ShellExtPropertyPageImpl&&) = delete;
    ShellExtPropertyPageImpl& operator=(const ShellExtPropertyPageImpl&) = delete;
    ShellExtPropertyPageImpl& operator=(ShellExtPropertyPageImpl&&) = delete;


    std::wstring GetDlgItemText(int nID) const
    {
        // Use ATL's implementation of GetDlgItemText.
        ATL::CString str;
        ATL::CSnapInPropertyPageImpl<T>::GetDlgItemText(nID, str);
        return str.GetString();
    }

#pragma warning(push)
#pragma warning(disable: 26433) // use override
    BEGIN_MSG_MAP(CShellExtPropertyPageImpl)
        CHAIN_MSG_MAP(ATL::CSnapInPropertyPageImpl<T>)
    END_MSG_MAP()
#pragma warning(pop)

private:
    int m_nRef{};
};

} // namespace msf
