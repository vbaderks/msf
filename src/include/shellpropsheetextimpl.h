//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "shellextinitimpl.h"
#include "catchhandler.h"

#pragma comment(lib, "comctl32")

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IShellPropSheetExtImpl :
    public IShellExtInitImpl,
    public IShellPropSheetExt
{
public:
    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) throw()
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID(), szRootKey);
    }

    IShellPropSheetExtImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellPropSheetExtImpl::IShellPropSheetExtImpl (instance=%p)\n", this);
    }

    ~IShellPropSheetExtImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellPropSheetExtImpl::~IShellPropSheetExtImpl (instance=%p)\n", this);
    }

    class CAddPage
    {
    public:
        CAddPage(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam) :
            m_pfnAddPage(pfnAddPage),
            m_lParam(lParam)
        {
        }

        void operator()(HPROPSHEETPAGE hPage) const
        {
            if (!m_pfnAddPage(hPage, m_lParam))
            {
                ATLVERIFY(::DestroyPropertySheetPage(hPage));
                RaiseException(E_FAIL);
            }
        }

    private:
        LPFNSVADDPROPSHEETPAGE m_pfnAddPage;
        LPARAM                 m_lParam;
    };

    // IShellPropSheetExt
    STDMETHOD(AddPages)(_In_ LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, L"IShellPropSheetExtImpl::IShellPropSheetExt::AddPages (instance=%p, pfnAddPage=%p, lParam=&p)\n", this, pfnAddPage, lParam);
            CAddPage addpage(pfnAddPage, lParam);

            static_cast<T*>(this)->OnAddPages(addpage, GetFilenames());
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(ReplacePage)(EXPPS /*uPageID*/, _In_ LPFNSVADDPROPSHEETPAGE /*pfnReplaceWith*/, LPARAM /*lParam*/)
    {
        // The Shell doesn't call this function for file class Property Sheets.
        // Only for control panel objects.
        MSF_TRACENOTIMPL(L"CPropSheetExtImpl::ReplacePage");
    }

protected:
    // OnAddPages must be implemented by derived classes.
    void OnAddPages(const CAddPage& /*addpages*/, const std::vector<CString>& /*filenames*/);
};

} // namespace MSF
