//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

inline ATL::CComPtr<IDataObject> CIDLData_CreateFromIDArray(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* pidlItems)
{
    ATL::CComPtr<IDataObject> dataobject;

    RaiseExceptionIfFailed(
        ::CIDLData_CreateFromIDArray(pidlFolder, cidl, pidlItems, &dataobject));

    return dataobject;
}

} // end namespace MSF
