//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "vvvfile.h"
#include "resource.h"
#include <msf.h>


class ATL_NO_VTABLE __declspec(uuid("D7DCE46C-98DB-4446-B1C0-E787325B1FBC")) IconOverlay :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<IconOverlay, &__uuidof(IconOverlay)>,
    public IconOverlayImpl<IconOverlay>
{
public:
    BEGIN_COM_MAP(IconOverlay)
        COM_INTERFACE_ENTRY(IShellIconOverlayIdentifier)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) noexcept
    {
        return IconOverlayImpl<IconOverlay>::UpdateRegistry(
            bRegister, IDR_ICONOVERLAY, L"VVV Sample ICON Overlay ShellExtension");
    }

    static bool IsMemberOfImpl(PCWSTR path, DWORD /*dwAttrib*/)
    {
        // Add an overlay for files that have the extension .vvvv in this sample.
        auto extension = PathFindExtension(path);
        return wcscmp(extension, L".vvvv") == 0;
    }

protected:
    IconOverlay() : IconOverlayImpl<IconOverlay>(IDI_ICON_OVERLAY)
    {
    }
};

OBJECT_ENTRY_AUTO(__uuidof(IconOverlay), IconOverlay)
