﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include "ole_string.h"

namespace msf {

/// <summary>Basic implementation functionality for a Browser Helper Object COM object (BHO).</summary>
/// <remarks>
/// Internet Explorer BHO objects must support the IObjectWithSite interface.
/// </remarks>
template <typename TDerived>
class __declspec(novtable) CBrowserHelperObjectImpl :
    public IObjectWithSiteImpl<TDerived>,
    public IDispEventImpl<1, TDerived, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
    static HRESULT __stdcall UpdateRegistry(BOOL bRegister, PCWSTR description, bool bNoExplorer) noexcept
    {
        COleString olestrCLSID;
        ATLVERIFY(SUCCEEDED(::StringFromCLSID(__uuidof(T), olestrCLSID.GetAddress())));

        _ATL_REGMAP_ENTRY regmapEntries[]
        {
            {L"DESCRIPTION", description},
            {L"CLSID", olestrCLSID},
            {L"NOEXPLORER", bNoExplorer ? L"1" : L"0"},
            {nullptr, nullptr}
        };

        return ATL::_pAtlModule->UpdateRegistryFromResource(IDR_BROWSERHELPEROBJECT, bRegister, regmapEntries);
    }

    HRESULT __stdcall SetSite(IUnknown* site) noexcept override
    {
        // Unadvise from current site.
        if (m_advised)
        {
            DispEventUnadvise(m_spUnkSite);
            m_advised = false;
        }

        if (site)
        {
            m_advised = SUCCEEDED(DispEventAdvise(site));
        }

        return IObjectWithSiteImpl<T>::SetSite(site);
    }

protected:
    ~CBrowserHelperObjectImpl()
    {
        // If COM object is destructed, make sure ref to site is cleared.
        CBrowserHelperObjectImpl<TDerived>::SetSite(nullptr);
    }

    bool IsAttachedSite(IUnknown* site) noexcept
    {
        return m_spUnkSite && m_spUnkSite.IsEqualObject(site);
    }

private:
    bool m_advised{};
};

}
