//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

#undef  INTERFACE
#define INTERFACE IShellFolderSearchableCallback
DECLARE_INTERFACE_IID_(IShellFolderSearchableCallback, IUnknown, "F98D8294-2BBC-11d2-8DBD-0000F87A556C")
{
    // *** IShellFolderSearchableCallback Methods ***
    STDMETHOD(RunBegin)(DWORD dwReserved) = 0;
    STDMETHOD(RunEnd)(DWORD dwReserved) = 0;
};
