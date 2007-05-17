//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <comdef.h>
#include "persistfolderptr.h"


_COM_SMARTPTR_TYPEDEF(IShellFolder2, __uuidof(IShellFolder2));


namespace MSF
{

class IShellFolder2Ptr : public ::IShellFolder2Ptr
{
public:
	explicit IShellFolder2Ptr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
		::IShellFolder2Ptr(clsid, pOuter, dwClsContext)
	{
	}


	IShellFolder2Ptr(const CLSID& clsid, const TCHAR* szFilename) :
		::IShellFolder2Ptr(clsid, NULL, CLSCTX_INPROC_SERVER)
	{
		Initialize(szFilename);
	}


	// Purpose: Constructs a smart-pointer from any other smart pointer.
	template<typename _OtherIID> IShellFolder2Ptr(const _com_ptr_t<_OtherIID>& p) :
		::IShellFolder2Ptr(p)
	{
	}


	// Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
	template<typename _InterfaceType> IShellFolder2Ptr(_InterfaceType* p) :
		::IShellFolder2Ptr(p)
	{
	}


	IEnumIDListPtr EnumObjects(HWND hwnd = NULL, SHCONTF grfFlags = 0)
	{
		IEnumIDListPtr enumidlist;
		RaiseExceptionIfFailed(GetInterfacePtr()->EnumObjects(hwnd, grfFlags, &enumidlist));

		return enumidlist;
	}


	void Initialize(const TCHAR* szFilename)
	{
		MSF::IPersistFolderPtr persistfolder(this);

		persistfolder.Initialize(szFilename);
	}
};

} // end of MSF namespace
