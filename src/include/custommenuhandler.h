//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "menuiteminfo.h"


namespace msf
{

// Purpose: handles owner draw menu items.
class CustomMenuHandler
{
public:
    virtual ~CustomMenuHandler()
    {
    }


    virtual void InitializeItemInfo(MenuItemInfo& menuiteminfo)
    {
        menuiteminfo.SetOwnerDraw();
    }


    // Purpose: called by OS to require the size of the menu item.
    virtual void Measure(MEASUREITEMSTRUCT& /*measureitem*/)
    {
    }


    // Purpose: called by the OS when the item must be draw.
    virtual void Draw(const DRAWITEMSTRUCT& /*drawitem*/)
    {
    }


    // Purpose: override this function to handle accelerator keys
    virtual bool OnMenuChar(HMENU /*hmenu*/, unsigned short /*nChar*/, LRESULT& /*lresult*/)
    {
        return false; // not handled.
    }
};

} // end namespace msf
