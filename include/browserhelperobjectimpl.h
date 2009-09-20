//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "olestring.h"

namespace MSF
{

/// <summary>Basic implementation functionality for a Browser Helper Object COM object (BHO).</summary>
/// <remarks>
/// Internet Explorer BHO objects must support the IObjectWithSite interface.
/// </remarks>
template <typename T>
class ATL_NO_VTABLE CBrowserHelperObjectImpl :
    public IObjectWithSiteImpl<T>,
    public IDispEventImpl<1, T, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
public:
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister, PCWSTR wszDescription, bool bNoExplorer) throw()
    {
        COleString olestrCLSID;
        ATLVERIFY(SUCCEEDED(::StringFromCLSID(__uuidof(T), olestrCLSID.GetAddress())));

        _ATL_REGMAP_ENTRY regmapEntries[] =
        {
            {L"DESCRIPTION", wszDescription},
            {L"CLSID", olestrCLSID},
            {L"NOEXPLORER", bNoExplorer ? L"1" : L"0"},
            {NULL, NULL}
        };

        return ATL::_pAtlModule->UpdateRegistryFromResource(IDR_BROWSERHELPEROBJECT, bRegister, regmapEntries);
    }

    CBrowserHelperObjectImpl() : m_bAdvised(false)
    {
    }

    ~CBrowserHelperObjectImpl()
    {
        // If COM object is destructed, make sure ref to site is cleared.
        SetSite(NULL);
    }

    STDMETHOD(SetSite)(IUnknown* pUnkSite)
    {
        // Unadvise from current site.
        if (m_bAdvised)
        {
            DispEventUnadvise(m_spUnkSite);
            m_bAdvised = false;
        }

        if (pUnkSite != NULL)
        {
            m_bAdvised = SUCCEEDED(DispEventAdvise(pUnkSite));
        }

        return IObjectWithSiteImpl<T>::SetSite(pUnkSite);
    }

protected:
    bool IsAttachedSite(IUnknown* pUnkSite) throw()
    {
        return m_spUnkSite != NULL && m_spUnkSite.IsEqualObject(pUnkSite);
    }

private:
    bool m_bAdvised;
};

}
