//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#undef  INTERFACE
#define INTERFACE IShellFolderSearchable
DECLARE_INTERFACE_IID_(IShellFolderSearchable, IUnknown, "4E1AE66C-204B-11d2-8DB3-0000F87A556C")
{
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, __out void **ppv) PURE;
	STDMETHOD_(ULONG,AddRef)  (THIS) PURE;
	STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** IShellFolderSearchable methods ***

	// FindString -
	//  The returned Shell folder's enumerator will have any
	//   search hits for the given search string.
	//  punkOnAsyncSearch will be QI'd for IShellFolderSearchableCallback
	STDMETHOD(FindString)(THIS_ LPCWSTR pwszTarget, __inout_opt DWORD *pdwFlags,
	                      __in_opt IUnknown *punkOnAsyncSearch, __out LPITEMIDLIST *ppidlOut) PURE;

	// CancelAsyncSearch -
	//   Begins the process of cancelling any pending
	//    asynchronous search from this pidl.
	//    When the search is actually cancelled, RunEnd will be called
	//   Returns: S_OK => cancelling, S_FALSE => not running
	STDMETHOD(CancelAsyncSearch) (THIS_ LPCITEMIDLIST pidlSearch, __inout_opt DWORD *pdwFlags) PURE;

	// InvalidateSearch -
	//   Makes this pidl no longer a valid portion of the Shell folder
	//    Also does some cleanup of any databases used in the search and
	//    will cause the eventual release of the callback
	//   May cause async search to be cancelled
	STDMETHOD(InvalidateSearch)  (THIS_ LPCITEMIDLIST pidlSearch, __inout_opt DWORD *pdwFlags) PURE;
};
