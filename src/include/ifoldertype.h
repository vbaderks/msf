//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

// IFolderType is requested by explorer.exe of Windows 7 and 8.
// It is an undocumented interface. The current definition is based on the work of Geoff Chappell.

#undef  INTERFACE
#define INTERFACE IFolderType

DECLARE_INTERFACE_IID_(IFolderType, IUnknown, "053B4A86-0DC9-40A3-B7ED-BC6A2E951F48")
{
    // A folder type id is a GUID. The purpose of this method is unclear.
    // It could used to return the GUID if one of the common sh folders is browsed.
    // Need to investigate what zipflr returns.
    STDMETHOD(GetFolderType)(THIS_ FOLDERTYPEID* folderTypeID) PURE;
};
