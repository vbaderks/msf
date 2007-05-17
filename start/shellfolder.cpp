#include "stdafx.h"
#include "../include/shellfolderimpl.h"
#include "../include/strutil.h"
#include "../include/queryinfo.h"
#include "enumidlist.h"
#include "item.h"
#include "resource.h"


// TODO: use the Create GUID' tool to create a new UUID
class DECLSPEC_UUID("637B6556-326D-478a-848A-AF147ECC600C") CShellFolder;


class ATL_NO_VTABLE CShellFolder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellFolder, &__uuidof(CShellFolder)>,
	public IShellFolderImpl<CShellFolder, CItem>
{
public:
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		// TODO: Add description and extension.
		return IShellFolderImpl<CShellFolder, CItem>::UpdateRegistry(
			IDR_SHELLFOLDER, IDR_SHELLFOLDER_WIN98, bRegister,
			L"<description>", L"<extension>", IDS_SHELLFOLDER_TYPE);
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CShellFolder)
		COM_INTERFACE_ENTRY2(IPersist, IPersistFolder2)
		COM_INTERFACE_ENTRY(IPersistFolder)
		COM_INTERFACE_ENTRY(IPersistFolder2)
		COM_INTERFACE_ENTRY(IPersistIDList)
		COM_INTERFACE_ENTRY(IShellFolder)  // included for backwards (win9x) compatiblity.
		COM_INTERFACE_ENTRY(IShellFolder2)
		COM_INTERFACE_ENTRY(IShellDetails) // included for backwards (win9x) compatiblity.
		COM_INTERFACE_ENTRY(IShellIcon)
		COM_INTERFACE_ENTRY(IDropTarget)   // enable drag and drop support.
	END_COM_MAP()


	// Purpose: called by MSF/shell when it want the current list of 
	//          all items  The shell will walk all IDs and then release the enum.
	CComPtr<IEnumIDList> CreateEnumIDList(HWND /*hwnd*/, DWORD /*grfFlags*/)
	{
		return CEnumIDList::CreateInstance(/* initialization parameters */);
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CShellFolder), CShellFolder)
