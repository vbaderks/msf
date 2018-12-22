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
    CustomMenuHandler() = default;
    virtual ~CustomMenuHandler() = default;

    CustomMenuHandler(const CustomMenuHandler&) = delete;
    CustomMenuHandler(CustomMenuHandler&&) = delete;
    CustomMenuHandler& operator=(const CustomMenuHandler&) = delete;
    CustomMenuHandler& operator=(CustomMenuHandler&&) = delete;

    virtual void InitializeItemInfo(MenuItemInfo& menuiteminfo)
    {
        menuiteminfo.SetOwnerDraw();
    }

    // Purpose: called by OS to require the size of the menu item.
    virtual void Measure(MEASUREITEMSTRUCT& /*measureItem*/)
    {
    }

    // Purpose: called by the OS when the item must be draw.
    virtual void Draw(const DRAWITEMSTRUCT& /*drawItem*/)
    {
    }

    // Purpose: override this function to handle accelerator keys
    virtual bool OnMenuChar(HMENU /*menu*/, unsigned short /*char*/, LRESULT& /*result*/)
    {
        return false; // not handled.
    }
};

} // end namespace msf
