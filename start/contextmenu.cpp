#include "stdafx.h"
#include "../include/contextmenuimpl.h"
#include "../include/util.h"
#include "shellfolderclsid.h"
#include "resource.h"

// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("7CA7AEDD-A32B-4d75-91F3-E48034091EFE") CContextMenu;

class ATL_NO_VTABLE CContextMenu :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CContextMenu, &__uuidof(CContextMenu)>,
    public IContextMenuImpl<CContextMenu>
{
public:
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
    {
        // TODO: Add description and extension.
        return IContextMenuImpl<CContextMenu>::UpdateRegistry(bRegister, IDR_CONTEXTMENU,
            L"<description>", L"<extension>");
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
        // TODO: register the extension.
    }

    // Purpose: called by the 'impl' class. Request to configure the menu
    void OnQueryContextMenu(IContextMenuImpl<CContextMenu>::CMenu& /*menu*/,
                            const vector<CString>& /*filenames*/)
    {
        // TODO: add menu items.
    }
};

OBJECT_ENTRY_AUTO(__uuidof(CContextMenu), CContextMenu)
