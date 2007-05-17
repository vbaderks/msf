//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "updateregistry.h"
#include "catchhandler.h"
#include "queryinfoimpl.h"
#include "shelluuids.h"


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IInfoTipImpl :
	public IPersistFile,
	public IQueryInfoImpl
{
public:
	// Registration function to register the infotip with a common shellfolder.
	static HRESULT WINAPI UpdateRegistry(UINT nResId, BOOL bRegister,
		PCWSTR szDescription, const CLSID& clsidShellFolder, PCWSTR szExtension) throw()
	{
		return UpdateRegistryFromResource(nResId, bRegister,
			szDescription, T::GetObjectCLSID(), clsidShellFolder, szExtension);
	}


	// Registration function to register the COM object + the root extension.
	static HRESULT WINAPI UpdateRegistryForRootExt(UINT nResId, BOOL bRegister,
		PCWSTR szDescription, PCWSTR szRootExt) throw()
	{
		return UpdateRegistryForRootExt(nResId, bRegister,
			szDescription, T::GetObjectCLSID(), szRootExt);
	}


	// Registration function to register the extension based on the root extension.
	static HRESULT WINAPI UpdateRegistryForExt(UINT nResId, BOOL bRegister,
		PCWSTR szRootType, PCWSTR szExtension) throw()
	{
		return ::UpdateRegistryForExt(nResId, bRegister,
			szRootType, szExtension);
	}


	// All-in-one registration function for 1 extenstion, call 'ForExt' to register
	// aditional functions.
	static HRESULT WINAPI UpdateRegistry(UINT nResIdRoot, UINT nResIdExt, BOOL bRegister,
		PCWSTR szDescription, PCWSTR szRootExt, PCWSTR szExtension) throw()
	{
		return ::UpdateRegistry(nResIdRoot, nResIdExt, bRegister,
			szDescription, T::GetObjectCLSID(), szRootExt, szExtension);
	}


	// IPersistFile
	// Note: A lot of functions are defined by the interface, but not for infotip objects.
	STDMETHOD(GetClassID)(LPCLSID)
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::GetClassID"));
	}


	STDMETHOD(IsDirty)()
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::IsDirty"));
	}


	STDMETHOD(Save)(LPCOLESTR, BOOL)
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::Save"));
	}


	STDMETHOD(SaveCompleted)(LPCOLESTR)
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::SaveCompleted"));
	}


	STDMETHOD(GetCurFile)(LPOLESTR*)
	{
		ATLTRACENOTIMPL(_T("IInfoTipImpl::GetCurFile"));
	}


	STDMETHOD(Load)(LPCOLESTR wszFilename, DWORD dwMode)
	{
		(dwMode); // unused in release.

#ifdef UNICODE
		ATLTRACE2(atlTraceCOM, 0, L"IInfoTipImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, dwMode, wszFilename);
#else
		ATLTRACE2(atlTraceCOM, 0, "IInfoTipImpl::Load (instance=%p, mode=%d, , filename=%S)\n", this, dwMode, wszFilename);
#endif

		try
		{
			// Note: CreateInfoTipText must be implemented by the derived class.
			SetInfoTipText(static_cast<T*>(this)->CreateInfoTipText(CW2T(wszFilename)));
			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}
};

} // namespace MSF
