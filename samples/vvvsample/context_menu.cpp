﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#include "pch.h"

#include "edit_with_notepad_command.h"
#include "about_msf_command.h"
#include "resource.h"

using std::make_unique;

class __declspec(novtable) __declspec(uuid("B498A476-9EB6-46c3-8146-CE77FF7EA063")) ContextMenu :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<ContextMenu, &__uuidof(ContextMenu)>,
    public ATL::IObjectWithSiteImpl<ContextMenu>,
    public msf::ContextMenuImpl<ContextMenu>
{
public:
    static HRESULT __stdcall UpdateRegistry(BOOL registerInRegistry) noexcept
    {
        return ContextMenuImpl<ContextMenu>::UpdateRegistry(registerInRegistry, IDR_CONTEXTMENU,
            L"VVV Sample Context Menu ShellExtension", wszVVVFileRootExt);
    }

    BEGIN_COM_MAP(ContextMenu)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu2)
        COM_INTERFACE_ENTRY(IContextMenu3)
        COM_INTERFACE_ENTRY(IObjectWithSite) // Used by Vista and up to set the site. The site can be used for menu commands like 'rename'
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    // Purpose: called by the implementation class. Request to configure the menu
    void QueryContextMenuCore(Menu& menu, const std::vector<std::wstring>& filenames) override
    {
        if (ContainsUnknownExtension(filenames))
            return; // only extend the menu when only .vvv files are selected.

        if (filenames.size() != 1)
            return; // only add to the context menu when 1 file is selected.

        auto menuVVV = menu.AddSubMenu(IDS_CONTEXTMENU_VVV_SUBMENU_HELP,
                                       make_unique<msf::SmallBitmapHandler>(IDS_CONTEXTMENU_VVV_SUBMENU, IDB_MENUICON));
        menuVVV.AddItem(IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD,
                        IDS_CONTEXTMENU_EDIT_WITH_NOTEPAD_HELP,
                        make_unique<EditWithNotepadCommand>());

        menuVVV.AddItem(IDS_CONTEXTMENU_ABOUT_MSF_HELP,
                        make_unique<AboutMSFCommand>(),
                        make_unique<msf::SmallBitmapHandler>(IDS_CONTEXTMENU_ABOUT_MSF, IDB_MENUICON));

        // ... optional add more sub menu's or more menu items.
    }

protected:
    ContextMenu()  noexcept
    {
        RegisterExtension(tszVVVExtension);
    }
};


OBJECT_ENTRY_AUTO(__uuidof(ContextMenu), ContextMenu)
