//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <string>

namespace msf {

class MenuItemInfo : public MENUITEMINFO
{
public:
    MenuItemInfo() noexcept : MENUITEMINFO()
    {
        cbSize = sizeof(MENUITEMINFO);
    }

    explicit MenuItemInfo(UINT id) noexcept : MenuItemInfo()
    {
        SetID(id);
    }

    MenuItemInfo(UINT id, std::wstring str) noexcept : MenuItemInfo(id)
    {
        SetString(std::move(str));
    }

    MenuItemInfo(UINT id, const std::wstring& str, HMENU hsubmenu) noexcept :  MenuItemInfo(id, str)
    {
        SetSubMenu(hsubmenu);
    }

    MenuItemInfo(UINT id, HMENU hsubmenu) noexcept : MenuItemInfo(id)
    {
        SetSubMenu(hsubmenu);
    }

    MenuItemInfo(const MenuItemInfo&) = delete;
    MenuItemInfo(MenuItemInfo&&) = delete;
    MenuItemInfo& operator=(const MenuItemInfo&) = delete;
    MenuItemInfo& operator=(MenuItemInfo&&) = delete;

    void SetID(UINT id) noexcept
    {
        fMask |= MIIM_ID;
        wID = id;
    }

    void SetString(std::wstring str) noexcept
    {
        fMask |= MIIM_TYPE;
        fType |= MFT_STRING;

        m_strCache = std::move(str);

#pragma warning(push)
#pragma warning(disable: 26465) // Don't cast away const
        dwTypeData = const_cast<wchar_t*>(m_strCache.c_str());
#pragma warning(pop)
    }

    void SetSubMenu(HMENU hsubmenu) noexcept
    {
        fMask |= MIIM_SUBMENU;
        hSubMenu = hsubmenu;
    }

    void SetOwnerDraw() noexcept
    {
        fType |= MFT_OWNERDRAW;
    }

    void SetCheckMarkBmps(HBITMAP hChecked, HBITMAP hUnchecked) noexcept
    {
        fMask |= MIIM_CHECKMARKS;
        hbmpChecked   = hChecked;
        hbmpUnchecked = hUnchecked;
    }

    void SetState(UINT uiState) noexcept
    {
        fMask |= MIIM_STATE;
        fState = uiState;
    }

private:
    // Member variables.
    mutable std::wstring m_strCache;
};

} // end namespace msf
