//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "menuiteminfo.h"
#include "strutil.h"


namespace msf
{

// Simple hmenu wrapper class. For more advanced functionality use WTL or MFC.
class CMenu final
{
public:
    explicit CMenu(bool bCreate = true) noexcept :
        m_hmenu(bCreate ? CreateMenu() : nullptr)
    {
    }

    ~CMenu()
    {
        ATLVERIFY(DestroyMenu(m_hmenu));
    }

    CMenu(const CMenu&) = delete;
    CMenu(CMenu&&) = delete;
    CMenu& operator=(const CMenu&) = delete;
    CMenu& operator=(CMenu&&) = delete;

    void AddItem(UINT id, UINT nIDText) const
    {
        AddItem(id, LoadResourceString(nIDText));
    }

    void AddItem(UINT id, const std::wstring& strText) const
    {
        const MenuItemInfo menuiteminfo(id, strText);
        InsertMenuItem(menuiteminfo, GetMenuItemCount());
    }

    void AddDefaultItem(UINT id, const std::wstring& strText) const
    {
        MenuItemInfo menuiteminfo(id, strText);
        menuiteminfo.SetState(MFS_DEFAULT);
        InsertMenuItem(menuiteminfo, GetMenuItemCount());
    }

    void InsertMenuItem(const MenuItemInfo& menuiteminfo, UINT uItem, bool bByPosition = true) const
    {
        RaiseLastErrorExceptionIf(!::InsertMenuItem(m_hmenu, uItem, bByPosition, &menuiteminfo));
    }

    UINT GetMenuItemCount() const
    {
        const int count = ::GetMenuItemCount(m_hmenu);
        RaiseLastErrorExceptionIf(count == -1);
        return static_cast<UINT>(count);
    }

    // ReSharper disable once CppNonExplicitConversionOperator
    operator HMENU() const noexcept
    {
        return m_hmenu;
    }

private:
    static HMENU CreateMenu()
    {
        const HMENU hmenu = ::CreateMenu();
        RaiseLastErrorExceptionIf(!hmenu);
        return hmenu;
    }

    // Member variables.
    HMENU m_hmenu;
};

} // end msf namespace
