﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//

#include "pch.h"

#include "resource.h"

class __declspec(novtable) __declspec(uuid("D7DCE46C-98DB-4446-B1C0-E787325B1FBC")) IconOverlay :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<IconOverlay, &__uuidof(IconOverlay)>,
    public msf::IconOverlayImpl<IconOverlay>
{
public:
    BEGIN_COM_MAP(IconOverlay)
        COM_INTERFACE_ENTRY(IShellIconOverlayIdentifier)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT __stdcall UpdateRegistry(BOOL registerInRegistry) noexcept
    {
        return IconOverlayImpl<IconOverlay>::UpdateRegistry(
            registerInRegistry, IDR_ICONOVERLAY, L"VVV Sample ICON Overlay ShellExtension");
    }

    static bool IsMemberOfImpl(PCWSTR path, DWORD /*dwAttrib*/) noexcept
    {
        // Add an overlay for files that have the extension .vvvv in this sample.
        PCWSTR const extension = PathFindExtension(path);
        return wcscmp(extension, L".vvvv") == 0;
    }

    IconOverlay(const IconOverlay&) = delete;
    IconOverlay(IconOverlay&&) = delete;
    IconOverlay& operator=(const IconOverlay&) = delete;
    IconOverlay& operator=(IconOverlay&&) = delete;

protected:
    IconOverlay() noexcept : IconOverlayImpl<IconOverlay>(IDI_ICON_OVERLAY)
    {
    }
    ~IconOverlay() = default;
};

OBJECT_ENTRY_AUTO(__uuidof(IconOverlay), IconOverlay)
