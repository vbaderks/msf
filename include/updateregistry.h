//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "olestring.h"
#include "strutil.h"


namespace MSF
{

inline void StringFromCLSID(REFCLSID clsid, COleString& olestrCLSID)
{
	ATLVERIFY(SUCCEEDED(::StringFromCLSID(clsid, olestrCLSID.GetAddress())));
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister, PCWSTR szDescription, const CLSID& clsid) throw()
{
	COleString olestrCLSID;
	StringFromCLSID(clsid, olestrCLSID);

	_ATL_REGMAP_ENTRY regmapEntries[] =
	{
		{L"DESCRIPTION", szDescription},
		{L"CLSID", olestrCLSID},
		{NULL, NULL}
	};

	return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


// Registration function to register the COM object + the root extension.
inline HRESULT UpdateRegistryForRootExt(UINT nResId, BOOL bRegister,
	PCWSTR szDescription, const CLSID& clsid, PCWSTR szRootExt) throw()
{
	COleString olestrCLSID;
	StringFromCLSID(clsid, olestrCLSID);

	_ATL_REGMAP_ENTRY regmapEntries[] =
	{
		{L"DESCRIPTION", szDescription},
		{L"CLSID", olestrCLSID},
		{L"ROOTTYPE", szRootExt},
		{NULL, NULL}
	};

	return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


// Registration function to register the extension based on the root extension.
inline HRESULT UpdateRegistryForExt(UINT nResId, BOOL bRegister,
	PCWSTR szRootExt, PCWSTR szExtension) throw()
{
	_ATL_REGMAP_ENTRY regmapEntries[] = 
	{
		{L"EXTENSION", szExtension},
		{L"ROOTTYPE", szRootExt},
		{NULL, NULL}
	};

	return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


// All-in-one registration function for 1 extenstion, call 'ForExt' to register aditional functions.
inline HRESULT UpdateRegistry(UINT nResIdRoot, UINT nResIdExt, BOOL bRegister,
	PCWSTR szDescription, const CLSID& clsid, PCWSTR szRootExt, PCWSTR szExtension) throw()
{
	// note: during unregistration try to unregister as much as possible.
	HRESULT hrFinal = S_OK;

	HRESULT hr = UpdateRegistryForRootExt(nResIdRoot, bRegister, szDescription, clsid, szRootExt);
	if (FAILED(hr))
		hrFinal = hr;

	hr = UpdateRegistryForExt(nResIdExt, bRegister, szRootExt, szExtension);
	if (FAILED(hr))
		hrFinal = hr;

	return hrFinal;
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister,
	PCWSTR szShellExtDescription, const CLSID& clsid, const CLSID& clsidShellFolder, PCWSTR szExtension) throw()
{
	COleString olestrCLSID;
	StringFromCLSID(clsid, olestrCLSID);

	COleString olestrClsidShellFolder;
	StringFromCLSID(clsidShellFolder, olestrClsidShellFolder);

	_ATL_REGMAP_ENTRY regmapEntries[] =
	{
		{L"DESCRIPTION", szShellExtDescription},
		{L"EXTENSION", szExtension},
		{L"CLSID", olestrCLSID},
		{L"CLSIDSHELLFOLDER", olestrClsidShellFolder},
		{NULL, NULL}
	};

	return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister,
	PCWSTR szShellExtDescription, const CLSID& clsid, PCWSTR szExtension, UINT nFriendlyTypeNameId) throw()
{
	COleString olestrCLSID;
	StringFromCLSID(clsid, olestrCLSID);

	CStringW strFriendlyTypenameId = ToStringW(nFriendlyTypeNameId);

	_ATL_REGMAP_ENTRY regmapEntries[] =
	{
		{L"DESCRIPTION", szShellExtDescription},
		{L"EXTENSION", szExtension},
		{L"CLSID", olestrCLSID},
		{L"FRIENDLYTYPENAME", strFriendlyTypenameId},
		{NULL, NULL}
	};

	return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}

} // namespace MSF
