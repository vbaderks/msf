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
    HRESULT TranslateAccelerator(MSG* /*pmsg*/)
    {
        return S_FALSE; // Tell Explorer to handle the accelerator.
    }

    HRESULT EnableModeless(BOOL /*fEnable*/)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::EnableModeless");
    }

    HRESULT UIActivate(UINT /*uState*/)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::UIActivate");
    }

    HRESULT Refresh()
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::Refresh");
    }

    HRESULT CreateViewWindow(IShellView *psvPrevious,
                             LPCFOLDERSETTINGS pfs,
                             IShellBrowser *psb,
                             RECT *prcView,
                             HWND *phWnd)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::CreateViewWindow");
    }

    HRESULT DestroyViewWindow()
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::DestroyViewWindow");
    }

    HRESULT GetCurrentInfo(LPFOLDERSETTINGS pfs)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetCurrentInfo");
    }

    HRESULT AddPropertySheetPages(DWORD dwReserved, LPFNSVADDPROPSHEETPAGE pfn, LPARAM lparam)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::AddPropertySheetPages");
    }

    HRESULT SaveViewState()
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::SaveViewState");
    }

    HRESULT SelectItem(LPCITEMIDLIST pidlItem, SVSIF uFlags)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::SelectItem");
    }

    HRESULT GetItemObject(UINT uItem, REFIID riid, void** ppv)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetItemObject");
    }

    // IShellView2
    HRESULT GetView(SHELLVIEWID* pvid, ULONG uView)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::GetView");
    }

    HRESULT CreateViewWindow2(LPSV2CVW2_PARAMS lpParams)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::CreateViewWindow2");
    }


    HRESULT HandleRename(LPCITEMIDLIST pidlNew)
    {
        ATLTRACENOTIMPL(L"IShellViewImpl::HandleRename");
    }

    HRESULT SelectAndPositionItem(LPCITEMIDLIST pidlItem, UINT uFlags,POINT *ppt)
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
