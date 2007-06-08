//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


// Note: very simple skeleton.


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IShellViewImpl :
	public IShellView2
{
public:
	IShellViewImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IShellViewImpl::IShellViewImpl (instance=%p)\n"), this);
	}


	~IShellViewImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IShellViewImpl::~IShellViewImpl (instance=%p)\n"), this);
	}


	// IShellView
	HRESULT TranslateAccelerator(MSG* /*pmsg*/)
	{
		return S_FALSE; // Tell Explorer to handle the accelerator.
	}


	HRESULT EnableModeless(BOOL /*fEnable*/)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::EnableModeless"));
	}


	HRESULT UIActivate(UINT /*uState*/)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::UIActivate"));
	}


	HRESULT Refresh()
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::Refresh"));
	}


	HRESULT CreateViewWindow(IShellView *psvPrevious,
	                         LPCFOLDERSETTINGS pfs,
	                         IShellBrowser *psb,
	                         RECT *prcView,
	                         HWND *phWnd)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::CreateViewWindow"));
	}


	HRESULT DestroyViewWindow()
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::DestroyViewWindow"));
	}


	HRESULT GetCurrentInfo(LPFOLDERSETTINGS pfs)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::GetCurrentInfo"));
	}


	HRESULT AddPropertySheetPages(DWORD dwReserved, LPFNSVADDPROPSHEETPAGE pfn, LPARAM lparam)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::AddPropertySheetPages"));
	}


	HRESULT SaveViewState()
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::SaveViewState"));
	}


	HRESULT SelectItem(LPCITEMIDLIST pidlItem, SVSIF uFlags)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::SelectItem"));
	}


	HRESULT GetItemObject(UINT uItem, REFIID riid, void** ppv)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::GetItemObject"));
	}


	// IShellView2
	HRESULT GetView(SHELLVIEWID* pvid, ULONG uView)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::GetView"));
	}


	HRESULT CreateViewWindow2(LPSV2CVW2_PARAMS lpParams)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::CreateViewWindow2"));
	}


	HRESULT HandleRename(LPCITEMIDLIST pidlNew)
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::HandleRename"));
	}


	HRESULT SelectAndPositionItem(LPCITEMIDLIST pidlItem, UINT uFlags,POINT *ppt);
	{
		ATLTRACENOTIMPL(_T("IShellViewImpl::SelectAndPositionItem"));
	}
};

} // namespace MSF
