//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "menu_item_info.h"

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

    virtual void InitializeItemInfo(MenuItemInfo& menuItemInfo) noexcept(false)
    {
        menuItemInfo.SetOwnerDraw();
    }

    // Purpose: called by OS to require the size of the menu item.
    virtual void Measure(MEASUREITEMSTRUCT& /*measureItem*/) noexcept(false)
    {
    }

    // Purpose: called by the OS when the item must be draw.
    virtual void Draw(const DRAWITEMSTRUCT& /*drawItem*/) noexcept(false)
    {
    }

    // Purpose: override this function to handle accelerator keys
    virtual bool OnMenuChar(HMENU /*menu*/, unsigned short /*char*/, LRESULT& /*result*/) noexcept(false)
    {
        return false; // not handled.
    }
};

} // end namespace msf
