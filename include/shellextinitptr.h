//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"

#if !defined(_WIN32_IE) || _WIN32_IE < 0x0501
#error IE 5.01 target is required for definition of shell interfaces.
#endif


namespace MSF
{

class IShellExtInitPtr :
	public ::IShellExtInitPtr
{
public:
	explicit IShellExtInitPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
		::IShellExtInitPtr(clsid, pOuter, dwClsContext)
	{
		// Note: base class will not throw for E_NOINTERFACE.
		RaiseExceptionIf(GetInterfacePtr() == NULL, E_NOINTERFACE);
	}


	void CreateInstance(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER)
	{
		RaiseExceptionIfFailed(__super::CreateInstance(clsid, pOuter, dwClsContext));
	}


	void Initialize(LPCITEMIDLIST pidlFolder, IDataObject* pdtobj, HKEY hkeyProgID)
	{
		RaiseExceptionIfFailed(GetInterfacePtr()->Initialize(pidlFolder, pdtobj, hkeyProgID));
	}
};

} // end of MSF namespace
