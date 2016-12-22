//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "olestring.h"
#include "strutil.h"


namespace MSF
{

inline void StringFromCLSID(REFCLSID clsid, COleString& olestrCLSID)
{
    ATLVERIFY(SUCCEEDED(::StringFromCLSID(clsid, olestrCLSID.GetAddress())));
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister, PCWSTR szDescription, const CLSID& clsid) noexcept
{
    COleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szDescription },
        { L"CLSID", olestrCLSID },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


/// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister,
    PCWSTR szDescription, const CLSID& clsid, PCWSTR szRootExt) noexcept
{
    COleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szDescription },
        { L"CLSID", olestrCLSID },
        { L"ROOTTYPE", szRootExt },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


/// <summary>Registration function to register the extension based on the root extension.</summary>
inline HRESULT UpdateRegistryConnectExtensionToProgId(UINT nResId, BOOL bRegister,
    PCWSTR szExtension, PCWSTR szProgId) noexcept
{
    _ATL_REGMAP_ENTRY regmapEntries[] = 
    {
        { L"EXTENSION", szExtension },
        { L"PROGID", szProgId },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister,
    PCWSTR szShellExtDescription, const CLSID& clsid, const CLSID& clsidShellFolder, PCWSTR szExtension) noexcept
{
    COleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    COleString olestrClsidShellFolder;
    StringFromCLSID(clsidShellFolder, olestrClsidShellFolder);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szShellExtDescription },
        { L"EXTENSION", szExtension },
        { L"CLSID", olestrCLSID },
        { L"CLSIDSHELLFOLDER", olestrClsidShellFolder },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister,
    PCWSTR szShellExtDescription, const CLSID& clsid, PCWSTR szExtension, UINT nFriendlyTypeNameId) noexcept
{
    COleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    CStringW strFriendlyTypenameId = ToStringW(nFriendlyTypeNameId);

    _ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szShellExtDescription },
        { L"EXTENSION", szExtension },
        { L"CLSID", olestrCLSID },
        { L"FRIENDLYTYPENAME", strFriendlyTypenameId },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}

} // namespace MSF
