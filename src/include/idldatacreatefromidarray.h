//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <shellapi.h>


namespace MSF
{

inline CComPtr<IDataObject> CIDLData_CreateFromIDArray(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* pidlItems)
{
    CComPtr<IDataObject> dataobject;

    RaiseExceptionIfFailed(
        ::CIDLData_CreateFromIDArray(pidlFolder, cidl, pidlItems, &dataobject));

    return dataobject;
}

} // end namespace MSF
