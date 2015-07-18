//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "util.h"

namespace MSF
{

/// <summary>Base class for the Clipboard format handlers.</summary>
class CCfHandler
{
public:
    CCfHandler(CLIPFORMAT clipformat, bool bCanGetData, bool bCanSetData) :
        m_clipformat(clipformat),
        m_bCanGetData(bCanGetData),
        m_bCanSetData(bCanSetData)
    {
    }

    CCfHandler(LPCTSTR lpszFormat, bool bCanGetData, bool bCanSetData) :
        m_clipformat(RegisterCf(lpszFormat)),
        m_bCanGetData(bCanGetData),
        m_bCanSetData(bCanSetData)
    {
    }

    virtual ~CCfHandler() throw()
    {
    }

    CLIPFORMAT GetClipFormat() const throw()
    {
        return m_clipformat;
    }

    bool CanGetData() const throw()
    {
        return m_bCanGetData;
    }

    bool CanSetData() const throw()
    {
        return m_bCanSetData;
    }

    virtual HRESULT Validate(const FORMATETC& formatetc) const throw()
    {
        if (formatetc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!IsBitSet(formatetc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (formatetc.lindex != -1)
            return DV_E_LINDEX;

        return S_OK;
    }

    virtual void GetData(const FORMATETC&, STGMEDIUM&) const
    {
    }

    virtual void SetData(const FORMATETC&, STGMEDIUM&, bool /*bRelease*/)
    {
    }

    bool IsValid(const FORMATETC& formatetc, const STGMEDIUM& stgmedium) const
    {
        return SUCCEEDED(Validate(formatetc)) && formatetc.tymed == stgmedium.tymed;
    }

private:
    CLIPFORMAT m_clipformat;
    bool       m_bCanGetData;
    bool       m_bCanSetData;
};

}
