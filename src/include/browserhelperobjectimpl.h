//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "olestring.h"

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
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, PCWSTR wszDescription, bool bNoExplorer) noexcept
    {
        COleString olestrCLSID;
        ATLVERIFY(SUCCEEDED(::StringFromCLSID(__uuidof(T), olestrCLSID.GetAddress())));

        _ATL_REGMAP_ENTRY regmapEntries[]
        {
            {L"DESCRIPTION", wszDescription},
            {L"CLSID", olestrCLSID},
            {L"NOEXPLORER", bNoExplorer ? L"1" : L"0"},
            {nullptr, nullptr}
        };

        return ATL::_pAtlModule->UpdateRegistryFromResource(IDR_BROWSERHELPEROBJECT, bRegister, regmapEntries);
    }

    HRESULT __stdcall SetSite(IUnknown* pUnkSite) noexcept override
    {
        // Unadvise from current site.
        if (m_bAdvised)
        {
            DispEventUnadvise(m_spUnkSite);
            m_bAdvised = false;
        }

        if (pUnkSite)
        {
            m_bAdvised = SUCCEEDED(DispEventAdvise(pUnkSite));
        }

        return IObjectWithSiteImpl<T>::SetSite(pUnkSite);
    }

protected:

    ~CBrowserHelperObjectImpl()
    {
        // If COM object is destructed, make sure ref to site is cleared.
        CBrowserHelperObjectImpl<TDerived>::SetSite(nullptr);
    }

    bool IsAttachedSite(IUnknown* pUnkSite) noexcept
    {
        return m_spUnkSite && m_spUnkSite.IsEqualObject(pUnkSite);
    }

private:
    bool m_bAdvised {false};
};

}
