//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "olestring.h"
#include "strutil.h"
#include <string>


namespace msf
{

inline void StringFromCLSID(REFCLSID classId, OleString& oleString) noexcept
{
    ATLVERIFY(SUCCEEDED(::StringFromCLSID(classId, oleString.GetAddress())));
}


inline HRESULT UpdateRegistryFromResource(uint32_t nResId, BOOL bRegister, PCWSTR description, const CLSID& clsid) noexcept
{
    OleString classId;
    StringFromCLSID(clsid, classId);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", description },
        { L"CLSID", classId },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


/// <summary>Registration function to register the COM object and a ProgId/extension.</summary>
inline HRESULT UpdateRegistryFromResource(uint32_t nResId, BOOL bRegister,
    PCWSTR szDescription, const CLSID& clsid, PCWSTR rootExt) noexcept
{
    OleString classId;
    StringFromCLSID(clsid, classId);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szDescription },
        { L"CLSID", classId },
        { L"ROOTTYPE", rootExt },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


/// <summary>Registration function to register the extension based on the root extension.</summary>
inline HRESULT UpdateRegistryConnectExtensionToProgId(uint32_t nResId, BOOL bRegister,
    PCWSTR extension, PCWSTR progId) noexcept
{
    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"EXTENSION", extension },
        { L"PROGID", progId },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


inline HRESULT UpdateRegistryFromResource(uint32_t nResId, BOOL bRegister,
    PCWSTR szShellExtDescription, const CLSID& clsid, const CLSID& clsidShellFolder, PCWSTR szExtension) noexcept
{
    OleString classId;
    StringFromCLSID(clsid, classId);

    OleString classIdShellFolder;
    StringFromCLSID(clsidShellFolder, classIdShellFolder);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szShellExtDescription },
        { L"EXTENSION", szExtension },
        { L"CLSID", classId },
        { L"CLSIDSHELLFOLDER", classIdShellFolder },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}


inline HRESULT UpdateRegistryFromResource(uint32_t nResId, BOOL bRegister,
    PCWSTR szShellExtDescription, const CLSID& clsid, PCWSTR szExtension, uint32_t nFriendlyTypeNameId) noexcept
{
    OleString classId;
    StringFromCLSID(clsid, classId);

    const auto friendlyTypenameId = std::to_wstring(nFriendlyTypeNameId);

    ATL::_ATL_REGMAP_ENTRY regmapEntries[] =
    {
        { L"DESCRIPTION", szShellExtDescription },
        { L"EXTENSION", szExtension },
        { L"CLSID", classId },
        { L"FRIENDLYTYPENAME", friendlyTypenameId.c_str() },
        { nullptr, nullptr }
    };

    return ATL::_pAtlModule->UpdateRegistryFromResource(nResId, bRegister, regmapEntries);
}

} // namespace msf
