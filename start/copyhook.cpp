#include "stdafx.h"
#include "../include/copyhookimpl.h"
#include "resource.h"


// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("042C0BE9-91C7-4e51-95B0-2B545844E518") CCopyHook;


class ATL_NO_VTABLE CCopyHook :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCopyHook, &__uuidof(CCopyHook)>,
	public ICopyHookImpl<CCopyHook>
{
public:
	BEGIN_COM_MAP(CCopyHook)
		COM_INTERFACE_ENTRY(ICopyHook)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		// TODO: Add a description and a name
		return ICopyHookImpl<CCopyHook>::UpdateRegistry(IDR_COPYHOOK, bRegister,
			L"<description>", L"<copyhook name>");
	}


	// ICopyHook overrides
	STDMETHOD_(UINT, CopyCallback)(HWND /*hwnd*/, UINT /*wFunc*/, UINT /*wFlags*/, LPCTSTR /*pszSrcFile*/, DWORD /*dwSrcAttribs*/,
	                               LPCTSTR /*pszDestFile*/, DWORD /*dwDestAttribs*/)
	{
		// TODO: Implement code to device when to return IDYES or IDNO.
		return IDYES;
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CCopyHook), CCopyHook)
