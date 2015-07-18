#include "stdafx.h"
#include "../include/shellpropsheetextimpl.h"
#include "propertypage.h"
#include "resource.h"


// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("98D4376B-9469-4f61-8F5F-63AE5AD2DA25") CShellPropSheetExt;


class ATL_NO_VTABLE CShellPropSheetExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellPropSheetExt, &__uuidof(CShellPropSheetExt)>,
	public IShellPropSheetExtImpl<CShellPropSheetExt>
{
public:
	static HRESULT WINAPI UpdateRegistry(BOOL /*bRegister*/) throw()
	{
		// TODO: decide to use this property sheet extension with a shellfolder or stand alone.
			// return IShellPropSheetExtImpl<CShellPropSheetExt>::UpdateRegistry(
			// IDR_PROPERTYSHEETEXT, bRegister,
			// L"<description>", __uuidof(CShellFolder), L"<extension>");
		// or
			// return IShellPropSheetExtImpl<CShellPropSheetExt>::UpdateRegistry(
			// IDR_EXTENSIONROOT, IDR_PROPERTYSHEETEXT, bRegister,
			// L"<description>", L"<root extension>", L"<extension>");
		return E_FAIL; // placeholder
	}

	BEGIN_COM_MAP(CShellPropSheetExt)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IShellPropSheetExt)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	CShellPropSheetExt()
	{
		// TODO: Pass supported extensions to the base class.
		RegisterExtension(_T("<extension>"));
	}


	// Purpose: called by MSF when it is time to add our pages to the propertysheet.
	void OnAddPages(const CAddPage& addpage, const vector<CString>& /*filenames*/)
	{
		// TODO: add pages (1 example is provided)
		addpage(CPropertyPage::CreateInstance());
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CShellPropSheetExt), CShellPropSheetExt)
