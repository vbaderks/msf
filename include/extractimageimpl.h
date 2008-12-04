//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "updateregistry.h"
#include "catchhandler.h"
#include <shlobj.h>


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IExtractImageImpl :
	public IPersistFile,
	public IExtractImage2
{
public:
	// Registration function to register the extension.
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


	IExtractImageImpl() :
		_hbitmap(NULL)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IExtractImageImpl::IExtractImageImpl (instance=%p)\n"), this);
	}


	~IExtractImageImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IExtractImageImpl::~IExtractImageImpl (instance=%p)\n"), this);

		Dispose();
	}


	// IPersistFile
	STDMETHOD(GetClassID)(CLSID* pClassID)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IExtractImageImpl::GetClassID\n"));

		if (pClassID == NULL)
			return E_POINTER;

		*pClassID = T::GetObjectCLSID();
		return S_OK;
	}


	STDMETHOD(IsDirty)()
	{
		MSF_TRACENOTIMPL(_T("IExtractImageImpl::IsDirty"));
	}


	STDMETHOD(Save)(LPCOLESTR, BOOL)
	{
		MSF_TRACENOTIMPL(_T("IExtractImageImpl::Save"));
	}


	STDMETHOD(SaveCompleted)(LPCOLESTR)
	{
		MSF_TRACENOTIMPL(_T("IExtractImageImpl::SaveCompleted"));
	}


	STDMETHOD(GetCurFile)(LPOLESTR*)
	{
		MSF_TRACENOTIMPL(_T("IExtractImageImpl::GetCurFile"));
	}


	STDMETHOD(Load)(LPCOLESTR wszFilename, DWORD dwMode)
	{
		(dwMode); // unused in release.

#ifdef UNICODE
		ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, dwMode, wszFilename);
#else
		ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::Load (instance=%p, mode=%d, , filename=%S)\n", this, dwMode, wszFilename);
#endif
		try
		{
			_strFilename = CW2T(wszFilename);
			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}


	// IExtractImage
	STDMETHOD(GetLocation)(LPWSTR pszPathBuffer, DWORD cch, DWORD* pdwPriority,
	                       const SIZE* psize, DWORD dwRecClrDepth, DWORD* pdwFlags)
	{
		try
		{
			ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::GetLocation (instance=%p, flags=%d)\n", this, *pdwFlags);

			Dispose();
			_hbitmap = static_cast<T*>(this)->CreateImage(*psize, dwRecClrDepth, *pdwFlags);

			lstrcpynW(pszPathBuffer, CT2W(static_cast<T*>(this)->GetPathBuffer()), static_cast<int>(cch));
			*pdwFlags |= IEIFLAG_CACHE;

			//  Note: The SDK docs are unclear if it passing a NULL pointer is allowed.
			if (pdwPriority != NULL)
			{
				*pdwPriority = IEIT_PRIORITY_NORMAL;
			}

			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}


	STDMETHOD(Extract)(HBITMAP* phBmpThumbnail)
	{
		ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::Extract (instance=%p)\n", this);

		if (_hbitmap == NULL)
			return E_FAIL;

		*phBmpThumbnail = _hbitmap;
		_hbitmap = NULL;

		return S_OK;
	}


	// IExtractImage2
	STDMETHOD(GetDateStamp)(FILETIME* pDateStamp)
	{
		ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::GetDateStamp (instance=%p, pdatastampe=%p)\n", this, pDateStamp);

		if (pDateStamp == NULL)
			return E_POINTER;

		WIN32_FILE_ATTRIBUTE_DATA fileattributedata;
		if (!GetFileAttributesEx(static_cast<T*>(this)->GetPathBuffer(), GetFileExInfoStandard, &fileattributedata))
			return E_FAIL;
		
		*pDateStamp = fileattributedata.ftLastWriteTime;
		return S_OK;
	}


	// Purpose: returns the identifier for the file that is used to 
	//          extract the image from. Can be used to optimize
	//          cases were the image is extracted from a different file 
	//          then set by IPersistFile::Load.
	CString GetPathBuffer()
	{
		return _strFilename;
	}

protected:

	void Dispose()
	{
		if (_hbitmap != NULL)
		{
			ATLVERIFY(DeleteObject(_hbitmap));
			_hbitmap = NULL;
		}
	}

	// member variables.
	CString _strFilename;
	HBITMAP _hbitmap;
};

} // namespace MSF
