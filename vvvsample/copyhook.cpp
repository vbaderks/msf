//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/copyhookimpl.h"
#include "../include/strutil.h"
#include "copyhookclsid.h"
#include "resource.h"


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
		return ICopyHookImpl<CCopyHook>::UpdateRegistry(bRegister, IDR_COPYHOOK,
			L"Sample ShellExtension CopyHook", L"VVV CopyHook");
	}


	// ICopyHook overrides
	STDMETHOD_(UINT, CopyCallback)(HWND hwnd, UINT wFunc, UINT /*wFlags*/, LPCTSTR pszSrcFile, DWORD /*dwSrcAttribs*/,
	                               LPCTSTR /*pszDestFile*/, DWORD /*dwDestAttribs*/)
	{
		if (wFunc == FO_DELETE && CString(pszSrcFile).Find(_T("VVV")) != -1)
		{
			return IsolationAwareMessageBox(hwnd, LoadString(IDS_COPYHOOK_QUESTION), 
				LoadString(IDS_COPYHOOK_CAPTION), MB_YESNOCANCEL);
		}
		else
		{
			return IDYES;
		}
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CCopyHook), CCopyHook)
