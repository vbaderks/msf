//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "menuiteminfo.h"
#include "strutil.h"


namespace MSF
{

// Simple hmenu wrapper class. For more advanced functionality use WTL or MFC.
class CMenu
{
public:
    explicit CMenu(bool bCreate = true) :
        m_hmenu(bCreate ? CreateMenu() : nullptr)
    {
    }


    ~CMenu()
    {
        ATLVERIFY(DestroyMenu(m_hmenu));
    }


    void AddItem(UINT id, UINT nIDText) const
    {
        AddItem(id, LoadString(nIDText));
    }


    void AddItem(UINT id, const CString& strText) const
    {
        CMenuItemInfo menuiteminfo(id, strText);

        InsertMenuItem(menuiteminfo, GetMenuItemCount());
    }


    void AddDefaultItem(UINT id, const CString& strText) const
    {
        CMenuItemInfo menuiteminfo(id, strText);

        menuiteminfo.SetState(MFS_DEFAULT);
        InsertMenuItem(menuiteminfo, GetMenuItemCount());
    }


    void InsertMenuItem(const CMenuItemInfo& menuiteminfo, UINT uItem, bool bByPosition = true) const
    {
        RaiseLastErrorExceptionIf(!::InsertMenuItem(m_hmenu, uItem, bByPosition, &menuiteminfo));
    }


    UINT GetMenuItemCount() const
    {
        int count = ::GetMenuItemCount(m_hmenu);
        RaiseLastErrorExceptionIf(count == -1);
        return static_cast<UINT>(count);
    }


    operator HMENU() const noexcept
    {
        return m_hmenu;
    }

private:

    static HMENU CreateMenu()
    {
        HMENU hmenu = ::CreateMenu();
        RaiseLastErrorExceptionIf(!hmenu);
        return hmenu;
    }

    // Member variables.
    HMENU m_hmenu;
};

} // end MSF namespace
