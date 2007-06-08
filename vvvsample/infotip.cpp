//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/infotipimpl.h"
#include "../include/strutil.h"
#include "infotipclsid.h"
#include "shellfolderclsid.h"
#include "vvvfile.h"
#include "resource.h"


class ATL_NO_VTABLE CInfoTip :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CInfoTip, &__uuidof(CInfoTip)>,
	public IInfoTipImpl<CInfoTip>
{
public:
	BEGIN_COM_MAP(CInfoTip)
		COM_INTERFACE_ENTRY(IPersistFile)
		COM_INTERFACE_ENTRY(IQueryInfo)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		return IInfoTipImpl<CInfoTip>::UpdateRegistry(IDR_INFOTIP, bRegister,
			L"Sample ShellExtension InfoTip", __uuidof(CShellFolder), wszVVVExtension);
	}


	// Purpose: called by the shell/MSF when it needs the text for the infotip.
	//          The string is used for the tooltip and the text in the statusbar.
	CString CreateInfoTipText(const TCHAR* szFilename)
	{
		CVVVFile vvvfile(szFilename);

		CString strTip = LoadString(IDS_SHELLEXT_LABEL) + _T(": ") + vvvfile.GetLabel() + _T("\n");
		strTip += LoadString(IDS_SHELLEXT_FILECOUNT) + _T(": ") +
			MSF::ToString(vvvfile.GetFileCount());

		return strTip;
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CInfoTip), CInfoTip)
