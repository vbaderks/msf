//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "contextmenuclsid.h"
#include "editwithnotepadcommand.h"
#include "aboutmsfcommand.h"
#include "resource.h"

#include "../include/contextmenuimpl.h"
#include "../include/smallbitmaphandler.h"


class ATL_NO_VTABLE CContextMenu :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CContextMenu, &__uuidof(CContextMenu)>,
    public IContextMenuImpl<CContextMenu>,
    public IObjectWithSiteImpl<CContextMenu>
{
public:
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) MSF_NOEXCEPT
    {
        return IContextMenuImpl<CContextMenu>::UpdateRegistry(bRegister, IDR_CONTEXTMENU,
            L"VVV Sample Context Menu ShellExtension", wszVVVFileRootExt);
    }

    BEGIN_COM_MAP(CContextMenu)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu2)
        COM_INTERFACE_ENTRY(IContextMenu3)
        COM_INTERFACE_ENTRY(IObjectWithSite) // Used by Vista and up to set the site. The site can be used for menu commands like 'rename'
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

        std::unique_ptr<CCustomMenuHandler> qsmallbitmaphandler(new CSmallBitmapHandler(IDS_CONTEXTMENU_VVV_SUBMENU, IDB_MENUICON));
        CMenu menuVVV = menu.AddSubMenu(IDS_CONTEXTMENU_VVV_SUBMENU_HELP, std::move(qsmallbitmaphandler));

        std::unique_ptr<CContextCommand> qeditwithnotepadcommand(new CEditWithNotepadCommand());
        menuVVV.AddItem(IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD,
                        IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD_HELP, std::move(qeditwithnotepadcommand));

        std::unique_ptr<CContextCommand> qaboutmsfcommand(new CAboutMSFCommand());
        std::unique_ptr<CCustomMenuHandler> qsmallbitmaphandler2(new CSmallBitmapHandler(IDS_CONTEXTMENU_ABOUT_MSF, IDB_MENUICON));
        menuVVV.AddItem(IDS_CONTEXTMENU_ABOUT_MSF_HELP, std::move(qaboutmsfcommand), std::move(qsmallbitmaphandler2));

        // ... optional add more submenu's or more menu items.
    }
};


OBJECT_ENTRY_AUTO(__uuidof(CContextMenu), CContextMenu)
