//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include "menu_item_info.h"
#include "str_util.h"

namespace msf
{

// Simple menu wrapper class. For more advanced functionality use the Windows Template Library (WTL).
class CMenu final
{
public:
    explicit CMenu(bool bCreate = true) noexcept :
        m_menu(bCreate ? CreateMenu() : nullptr)
    {
    }

    ~CMenu()
    {
        ATLVERIFY(DestroyMenu(m_menu));
    }

    CMenu(const CMenu&) = delete;
    CMenu(CMenu&&) = delete;
    CMenu& operator=(const CMenu&) = delete;
    CMenu& operator=(CMenu&&) = delete;

    void AddItem(uint32_t id, uint32_t nIDText) const
    {
        AddItem(id, LoadResourceString(nIDText));
    }

    void AddItem(uint32_t id, const std::wstring& strText) const
    {
        const MenuItemInfo menuiteminfo(id, strText);
        InsertMenuItem(menuiteminfo, GetMenuItemCount());
    }

    void AddDefaultItem(uint32_t id, const std::wstring& text) const
    {
        MenuItemInfo info(id, text);
        info.SetState(MFS_DEFAULT);
        InsertMenuItem(info, GetMenuItemCount());
    }

    void InsertMenuItem(const MenuItemInfo& info, uint32_t uItem, bool byPosition = true) const
    {
        RaiseLastErrorExceptionIf(!::InsertMenuItem(m_menu, uItem, byPosition, &info));
    }

    uint32_t GetMenuItemCount() const
    {
        const int count = ::GetMenuItemCount(m_menu);
        RaiseLastErrorExceptionIf(count == -1);
        return static_cast<uint32_t>(count);
    }

    // ReSharper disable once CppNonExplicitConversionOperator
    operator HMENU() const noexcept
    {
        return m_menu;
    }

private:
    static HMENU CreateMenu()
    {
        const HMENU menu = ::CreateMenu();
        RaiseLastErrorExceptionIf(!menu);
        return menu;
    }

    // Member variables.
    HMENU m_menu;
};

} // end msf namespace
