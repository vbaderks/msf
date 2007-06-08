//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "../include/contextmenuimpl.h"
#include "../include/smallbitmaphandler.h"
#include "../include/util.h"
#include "contextmenuclsid.h"
#include "shellfolderclsid.h"
#include "editwithnotepadcommand.h"
#include "aboutmsfcommand.h"
#include "resource.h"


class ATL_NO_VTABLE CContextMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CContextMenu, &__uuidof(CContextMenu)>,
	public IContextMenuImpl<CContextMenu>
{
public:
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		return IContextMenuImpl<CContextMenu>::UpdateRegistry(IDR_CONTEXTMENU, 
			bRegister, L"Sample ShellExtension ContextMenu", __uuidof(CShellFolder), wszVVVExtension);
	}

	BEGIN_COM_MAP(CContextMenu)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IContextMenu)
		COM_INTERFACE_ENTRY(IContextMenu2)
		COM_INTERFACE_ENTRY(IContextMenu3)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	CContextMenu()
	{
		RegisterExtension(tszVVVExtension);
	}


	// Purpose: called by the 'impl' class. Request to configure the menu
	void OnQueryContextMenu(IContextMenuImpl<CContextMenu>::CMenu& menu,
	                        const vector<CString>& filenames)
	{
		if (ContainsUnknownExtension(filenames))
			return; // only extend the menu when only .vvv files are selected.

		if (filenames.size() != 1)
			return; // only add to the context menu when 1 file is selected.

		CCustomMenuHandlerPtr qsmallbitmaphandler(new CSmallBitmapHandler(IDS_CONTEXTMENU_VVV_SUBMENU, IDB_MENUICON));
		CMenu menuVVV = menu.AddSubMenu(IDS_CONTEXTMENU_VVV_SUBMENU_HELP, qsmallbitmaphandler);

		CContextCommandPtr qeditwithnotepadcommand(new CEditWithNotepadCommand());
		menuVVV.AddItem(IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD,
		                IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD_HELP, qeditwithnotepadcommand);

		CContextCommandPtr qaboutmsfcommand(new CAboutMSFCommand());
		CCustomMenuHandlerPtr qsmallbitmaphandler2(new CSmallBitmapHandler(IDS_CONTEXTMENU_ABOUT_MSF, IDB_MENUICON));
		menuVVV.AddItem(IDS_CONTEXTMENU_ABOUT_MSF_HELP, qaboutmsfcommand, qsmallbitmaphandler2);

		// ... optional add more submenu's or more menu items.
	}
};


OBJECT_ENTRY_AUTO(__uuidof(CContextMenu), CContextMenu)
