//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE CShellExtPropertyPageImpl : public CSnapInPropertyPageImpl<T>
{
public:
    static UINT CALLBACK PropPageCallback(HWND hWnd, UINT uMsg, LPPROPSHEETPAGE ppsp)
    {
        CShellExtPropertyPageImpl<T>* pT = reinterpret_cast<T*>(ppsp->lParam);

        switch (uMsg)
        {
        case PSPCB_ADDREF:
            ATLTRACE2(atlTraceCOM, 0, L"CShellExtPropertyPageImpl::Callback (instance=%p, uMsg=AddRef)\n", pT);
            pT->m_nRef++; // only 5.80 (IE5) and up sends 'addref'.
            break;

        case PSPCB_CREATE:
            return CSnapInPropertyPageImpl<T>::PropPageCallback(hWnd, uMsg, ppsp);

        case PSPCB_RELEASE:
            ATLTRACE2(atlTraceCOM, 0, L"CShellExtPropertyPageImpl::Callback (instance=%p, uMsg=Release)\n", pT);
            pT->m_nRef--;
            if (pT->m_nRef <= 0)
                delete pT;
            break;

        default:
            ATLASSERT(!"Detected unexpected PSPCB uMsg");
            break;
        }

        return 1;
    }

    CShellExtPropertyPageImpl(LPCTSTR lpszTitle = nullptr) :
        CSnapInPropertyPageImpl<T>(lpszTitle),
        m_nRef(0)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CShellExtPropertyPageImpl::CShellExtPropertyPageImpl (instance=%p)\n", this);
        _pAtlModule->Lock(); // propertypage is not a COM object, but DLL must stay in memory.
    }

    ~CShellExtPropertyPageImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"CShellExtPropertyPageImpl::~CShellExtPropertyPageImpl (instance=%p)\n", this);
        _pAtlModule->Unlock();
    }

    BEGIN_MSG_MAP(CShellExtPropertyPageImpl)
        CHAIN_MSG_MAP(CSnapInPropertyPageImpl<T>)
    END_MSG_MAP()

private:
    int m_nRef;
};

} // namespace MSF
