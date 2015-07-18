//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

// ITopViewAwareItem is requested by explorer.exe of Windows 7 and 8. It is an undocumented interface.

#undef  INTERFACE
#define INTERFACE   ITopViewAwareItem

DECLARE_INTERFACE_IID_(ITopViewAwareItem, IUnknown, "8279FEB8-5CA4-45C4-BE27-770DCDEA1DEB")
{
    // No functions defined: specifications unclear.
};
