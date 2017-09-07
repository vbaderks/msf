//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "updateregistry.h"
#include "shellextinitimpl.h"

#pragma comment(lib, "comctl32")

namespace MSF
{

template <typename T>
class __declspec(novtable) ShellPropSheetExtImpl :
    public IShellExtInitImpl,
    public IShellPropSheetExt
{
public:
    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR szDescription, PCWSTR szRootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister, szDescription, T::GetObjectCLSID(), szRootKey);
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
    STDMETHOD(AddPages)(_In_ LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam) override
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, L"ShellPropSheetExtImpl::AddPages (instance=%p, pfnAddPage=%p, lParam=&p)\n", this, pfnAddPage, lParam);
            CAddPage addpage(pfnAddPage, lParam);

            AddPagesCore(addpage, GetFilenames());
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(ReplacePage)(EXPPS /*uPageID*/, _In_ LPFNSVADDPROPSHEETPAGE /*pfnReplaceWith*/, LPARAM /*lParam*/) override
    {
        // The Shell doesn't call this function for file class Property Sheets.
        // Only for control panel objects.
        ATLTRACENOTIMPL(L"ShellPropSheetExtImpl::ReplacePage");
    }

protected:
    ShellPropSheetExtImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ShellPropSheetExtImpl::ShellPropSheetExtImpl (instance=%p)\n", this);
    }

    ~ShellPropSheetExtImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ShellPropSheetExtImpl::~ShellPropSheetExtImpl (instance=%p)\n", this);
    }

    virtual void AddPagesCore(const CAddPage& /*addpages*/, const std::vector<std::wstring>& /*filenames*/) = 0;
};

} // namespace MSF
