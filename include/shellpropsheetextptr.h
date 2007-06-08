//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class IShellPropSheetExtPtr :
	public ::IShellPropSheetExtPtr
{
public:
	explicit IShellPropSheetExtPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_ALL) :
		::IShellPropSheetExtPtr(clsid, pOuter, dwClsContext)
	{
	}


	// Purpose: Constructs a smart-pointer from any other smart pointer.
	template<typename _OtherIID> IShellPropSheetExtPtr(const _com_ptr_t<_OtherIID>& p) :
		::IShellPropSheetExtPtr(p)
	{
	}


	// Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
	template<typename _InterfaceType> IShellPropSheetExtPtr(_InterfaceType* p) :
		::IShellPropSheetExtPtr(p)
	{
	}


	void CreateInstance(const CLSID& clsid)
	{
		RaiseExceptionIfFailed(__super::CreateInstance(clsid));
	}


	void AddPages(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
	{
		RaiseExceptionIfFailed(GetInterfacePtr()->AddPages(pfnAddPage, lParam));
	}


	void AddPages(LPFNSVADDPROPSHEETPAGE pfnAddPage, void* pparam)
	{
		AddPages(pfnAddPage, reinterpret_cast<LPARAM>(pparam));
	}
};

} // end of MSF namespace
