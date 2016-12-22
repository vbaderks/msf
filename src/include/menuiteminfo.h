//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

class CMenuItemInfo : public MENUITEMINFO
{
public:

    CMenuItemInfo() MSF_NOEXCEPT
    {
        cbSize = sizeof(MENUITEMINFO);
        fMask = 0;
        fType = 0;
    }


    CMenuItemInfo(UINT id) MSF_NOEXCEPT : CMenuItemInfo()
    {
        SetID(id);
    }


    CMenuItemInfo(UINT id, const CString& str) MSF_NOEXCEPT : CMenuItemInfo(id)
    {
        SetString(str);
    }


    CMenuItemInfo(UINT id, const CString& str, HMENU hsubmenu) MSF_NOEXCEPT :  CMenuItemInfo(id, str)
    {
        SetSubMenu(hsubmenu);
    }


    CMenuItemInfo(UINT id, HMENU hsubmenu) MSF_NOEXCEPT : CMenuItemInfo(id)
    {
        SetSubMenu(hsubmenu);
    }

    CMenuItemInfo(const CMenuItemInfo&) = delete;
    CMenuItemInfo& operator=(const CMenuItemInfo&) = delete;

    void SetID(UINT id) MSF_NOEXCEPT
    {
        fMask |= MIIM_ID;
        wID = id;
    }


    void SetString(const CString& str) MSF_NOEXCEPT
    {
        fMask |= MIIM_TYPE;
        fType |= MFT_STRING;

        m_strCache = str;
        dwTypeData = const_cast<wchar_t*>(m_strCache.GetString());
    }


    void SetSubMenu(HMENU hsubmenu) MSF_NOEXCEPT
    {
        fMask |= MIIM_SUBMENU;
        hSubMenu = hsubmenu;
    }


    void SetOwnerDraw() MSF_NOEXCEPT
    {
        fType |= MFT_OWNERDRAW;
    }


    void SetCheckMarkBmps(HBITMAP hChecked, HBITMAP hUnchecked) MSF_NOEXCEPT
    {
        fMask |= MIIM_CHECKMARKS;
        hbmpChecked   = hChecked;
        hbmpUnchecked = hUnchecked;
    }


    void SetState(UINT uiState) MSF_NOEXCEPT
    {
        fMask |= MIIM_STATE;
        fState = uiState;
    }

private:

    // Member variables.
    CString m_strCache;
};

} // end namespace MSF
