//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "updateregistry.h"
#include "shellextinitimpl.h"
#include "catchhandler.h"


#pragma comment(lib, "comctl32")


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IShellPropSheetExtImpl :
	public IShellExtInitImpl,
	public IShellPropSheetExt
{
public:
	// Registration function to register with a common shellfolder.
	static HRESULT WINAPI UpdateRegistry(UINT nResId, BOOL bRegister,
		PCWSTR szDescription, const CLSID& clsidShellFolder, PCWSTR szExtension) throw()
	{
		return UpdateRegistryFromResource(nResId, bRegister,
			szDescription, T::GetObjectCLSID(), clsidShellFolder, szExtension);
	}


	static HRESULT WINAPI UpdateRegistryForRootExt(UINT nResId, BOOL bRegister,
		PCWSTR szDescription, PCWSTR szRootExt) throw()
	{
		return ::UpdateRegistryForRootExt(nResId, bRegister,
			szDescription, T::GetObjectCLSID(), szRootExt);
	}


	static HRESULT WINAPI UpdateRegistryForExt(UINT nResId, BOOL bRegister,
		PCWSTR szRootExt, PCWSTR szExtension) throw()
	{
		return ::UpdateRegistryForExt(nResId, bRegister,
			szRootExt, szExtension);
	}


	static HRESULT WINAPI UpdateRegistry(UINT nResIdRoot, UINT nResIdExt, BOOL bRegister,
		PCWSTR szDescription, PCWSTR szRootExt, PCWSTR szExtension) throw()
	{
		return ::UpdateRegistry(nResIdRoot, nResIdExt, bRegister,
			szDescription, T::GetObjectCLSID(), szRootExt, szExtension);
	}


	IShellPropSheetExtImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IShellPropSheetExtImpl::Constructor (instance=%p)\n"), this);
	}


	~IShellPropSheetExtImpl()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IShellPropSheetExtImpl::~Destructor (instance=%p)\n"), this);
	}


	class CAddPage
	{
	public:
		CAddPage(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam) :
			m_pfnAddPage(pfnAddPage),
			m_lParam(lParam)
		{
		}


		void operator()(HPROPSHEETPAGE hPage) const
		{
			if (!m_pfnAddPage(hPage, m_lParam))
			{
				ATLVERIFY(::DestroyPropertySheetPage(hPage));
				RaiseException(E_FAIL);
			}
		}

	private:

		// Member variables.
		LPFNSVADDPROPSHEETPAGE m_pfnAddPage;
		LPARAM                 m_lParam;
	};


	// IShellPropSheetExt
	STDMETHOD(AddPages)(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
	{
		try
		{
			CAddPage addpage(pfnAddPage, lParam);

			static_cast<T*>(this)->OnAddPages(addpage, GetFilenames());
			return S_OK;
		}
		MSF_COM_CATCH_HANDLER()
	}


	STDMETHOD(ReplacePage)(EXPPS /*uPageID*/, LPFNSVADDPROPSHEETPAGE /*pfnReplaceWith*/, LPARAM /*lParam*/)
	{
		// The Shell doesn't call this function for file class Property Sheets.
		// Only for control panel objects.
		ATLTRACENOTIMPL(_T("CPropSheetExtImpl::ReplacePage"));
	}

protected:

	// OnAddPages must be implemented by derived classes.
	void OnAddPages(const CAddPage& /*addpages*/, const std::vector<CString>& /*filenames*/);
};

} // namespace MSF
