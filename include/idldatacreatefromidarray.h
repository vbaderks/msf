//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

// Purpose: The CIDLData_CreateFromIDArray is documented but the prototype and 
//          the import library are missing (Platform SDK WinXP SP2)

#include "msfbase.h"
#include <shellapi.h>

extern "C"
SHSTDAPI
CIDLData_CreateFromIDArray(const ITEMIDLIST* pidlFolder, UINT cidl, const ITEMIDLIST** pidlItems, IDataObject** ppdataobject);


#pragma comment(lib, "shell32missing")


namespace MSF
{

inline CComPtr<IDataObject> CIDLData_CreateFromIDArray(const ITEMIDLIST* pidlFolder, UINT cidl, const ITEMIDLIST** pidlItems)
{
	CComPtr<IDataObject> dataobject;

	RaiseExceptionIfFailed(
		::CIDLData_CreateFromIDArray(pidlFolder, cidl, pidlItems, &dataobject));

	return dataobject;
}

} // end namespace MSF
