//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

// Note: very simple skeleton.

namespace msf
{

template <typename T>
class __declspec(novtable) IShellViewImpl :
    public IShellView2
{
public:
    // IShellView
    HRESULT TranslateAccelerator(MSG* /*pmsg*/) override
    {
        return S_FALSE; // Tell Explorer to handle the accelerator.
    }

    HRESULT EnableModeless(BOOL /*fEnable*/) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::EnableModeless");
    }

    HRESULT UIActivate(UINT /*uState*/) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::UIActivate");
    }

    HRESULT Refresh() override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::Refresh");
    }

    HRESULT CreateViewWindow(IShellView*,
                             LPCFOLDERSETTINGS,
                             IShellBrowser*,
                             RECT*,
                             HWND*) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::CreateViewWindow");
    }

    HRESULT DestroyViewWindow() override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::DestroyViewWindow");
    }

    HRESULT GetCurrentInfo(LPFOLDERSETTINGS pfs) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetCurrentInfo");
    }

    HRESULT AddPropertySheetPages(DWORD dwReserved, LPFNSVADDPROPSHEETPAGE pfn, LPARAM lparam) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::AddPropertySheetPages");
    }

    HRESULT SaveViewState() override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::SaveViewState");
    }

    HRESULT SelectItem(LPCITEMIDLIST pidlItem, SVSIF uFlags)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::SelectItem");
    }

    HRESULT GetItemObject(UINT uItem, REFIID riid, void** ppv) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetItemObject");
    }

    // IShellView2
    HRESULT GetView(SHELLVIEWID* pvid, ULONG uView) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetView");
    }

    HRESULT CreateViewWindow2(LPSV2CVW2_PARAMS) override
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::CreateViewWindow2");
    }


    HRESULT HandleRename(LPCITEMIDLIST)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::HandleRename");
    }

    HRESULT SelectAndPositionItem(LPCITEMIDLIST, UINT, POINT*)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::SelectAndPositionItem");
    }

protected:

    IShellViewImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellViewImpl::IShellViewImpl (instance=%p)\n", this);
    }

    ~IShellViewImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IShellViewImpl::~IShellViewImpl (instance=%p)\n", this);
    }
};

} // namespace msf
