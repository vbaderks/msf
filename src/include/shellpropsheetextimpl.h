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

namespace msf
{

template <typename T>
class __declspec(novtable) ShellPropSheetExtImpl :
    public IShellExtInitImpl,
    public IShellPropSheetExt
{
public:
    ShellPropSheetExtImpl(const ShellPropSheetExtImpl&) = delete;
    ShellPropSheetExtImpl(ShellPropSheetExtImpl&&) = delete;
    ShellPropSheetExtImpl& operator=(const ShellPropSheetExtImpl&) = delete;
    ShellPropSheetExtImpl& operator=(ShellPropSheetExtImpl&&) = delete;

    /// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, UINT nResId,
        PCWSTR description, PCWSTR rootKey) noexcept
    {
        return UpdateRegistryFromResource(nResId, bRegister, description, T::GetObjectCLSID(), rootKey);
    }

    class CAddPage
    {
    public:
        CAddPage(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam) noexcept :
            m_pfnAddPage(pfnAddPage),
            m_lParam(lParam)
        {
        }

        void operator()(HPROPSHEETPAGE page) const
        {
            if (!m_pfnAddPage(page, m_lParam))
            {
                ATLVERIFY(::DestroyPropertySheetPage(page));
                RaiseException(E_FAIL);
            }
        }

    private:
        LPFNSVADDPROPSHEETPAGE m_pfnAddPage;
        LPARAM                 m_lParam;
    };

    // IShellPropSheetExt
    HRESULT __stdcall AddPages(_In_ LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam) override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellPropSheetExtImpl::AddPages (instance=%p, pfnAddPage=%p, lParam=&p)\n", this, pfnAddPage, lParam);
            const CAddPage addpage(pfnAddPage, lParam);

            AddPagesCore(addpage, GetFilenames());
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall ReplacePage(EXPPS /*uPageID*/, _In_ LPFNSVADDPROPSHEETPAGE /*pfnReplaceWith*/, LPARAM /*lParam*/) override
    {
        // The Shell doesn't call this function for file class Property Sheets.
        // Only for control panel objects.
        ATLTRACENOTIMPL(L"ShellPropSheetExtImpl::ReplacePage");
    }

protected:
    ShellPropSheetExtImpl() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellPropSheetExtImpl::ShellPropSheetExtImpl (instance=%p)\n", this);
    }

    ~ShellPropSheetExtImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ShellPropSheetExtImpl::~ShellPropSheetExtImpl (instance=%p)\n", this);
    }

    virtual void AddPagesCore(const CAddPage& /*addPages*/, const std::vector<std::wstring>& /*filenames*/) = 0;
};

} // namespace msf
