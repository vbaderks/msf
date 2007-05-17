#include "stdafx.h"
#include "resource.h"
#include "../include/columnproviderimpl.h"


// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("83F9C7E4-4B6D-41f5-96D8-BC56B9836DCA") CColumnProvider;


class ATL_NO_VTABLE CColumnProvider :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CColumnProvider, &__uuidof(CColumnProvider)>,
	public IColumnProviderImpl<CColumnProvider>
{
public:
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		// TODO: Add description.
		return IColumnProviderImpl<CColumnProvider>::UpdateRegistry(
			IDR_COLUMNPROVIDER, bRegister, L"<description>");
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CColumnProvider)
		COM_INTERFACE_ENTRY(IColumnProvider)
	END_COM_MAP()


	void OnInitialize(PCWSTR /*wszFolder*/)
	{
		// TODO: register supported columns.
	}


	// Purpose: this function is called by MSF when it needs column info for a file.
	void GetAllColumnInfo(const CString& /*strFilename*/, vector<CString>& /*strColumnInfos*/)
	{
		// TODO: fill strColumnInfos
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CColumnProvider), CColumnProvider)
