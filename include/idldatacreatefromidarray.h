//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <shellapi.h>

// Purpose: The CIDLData_CreateFromIDArray is documented but the prototype and 
//          the import library are missing (Platform SDK WinXP SP2)
//          The Windows SKD 8.0 does contain the definition.
#if _MSC_VER < 1400

extern "C"
SHSTDAPI
CIDLData_CreateFromIDArray(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* pidlItems, IDataObject** ppdataobject);

#pragma comment(lib, "shell32missing")

#endif

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
