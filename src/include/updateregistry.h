//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "olestring.h"
#include "strutil.h"


namespace msf
{

inline void StringFromCLSID(REFCLSID clsid, OleString& olestrCLSID) noexcept
{
    ATLVERIFY(SUCCEEDED(::StringFromCLSID(clsid, olestrCLSID.GetAddress())));
}


inline HRESULT UpdateRegistryFromResource(UINT nResId, BOOL bRegister, PCWSTR szDescription, const CLSID& clsid) noexcept
{
    OleString olestrCLSID;
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
    OleString olestrCLSID;
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
    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
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
    OleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    OleString olestrClsidShellFolder;
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
    OleString olestrCLSID;
    StringFromCLSID(clsid, olestrCLSID);

    const ATL::CStringW strFriendlyTypenameId = ToStringW(nFriendlyTypeNameId);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szShellExtDescription },
        { L"EXTENSION", szExtension },
        { L"CLSID", olestrCLSID },
        { L"FRIENDLYTYPENAME", strFriendlyTypenameId },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}

} // namespace msf
