//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "util.h"

namespace msf
{

/// <summary>Base class for the Clipboard format handlers.</summary>
class ClipboardFormatHandler
{
public:
    ClipboardFormatHandler(const ClipboardFormatHandler&) = delete;
    ClipboardFormatHandler(ClipboardFormatHandler&&) = delete;
    virtual ~ClipboardFormatHandler() = default;
    ClipboardFormatHandler& operator=(const ClipboardFormatHandler&) = delete;
    ClipboardFormatHandler& operator=(ClipboardFormatHandler&&) = delete;

    CLIPFORMAT GetClipFormat() const noexcept
    {
        return m_clipFormat;
    }

    bool CanGetData() const noexcept
    {
        return m_bCanGetData;
    }

    bool CanSetData() const noexcept
    {
        return m_bCanSetData;
    }

    virtual HRESULT Validate(const FORMATETC& formatEtc) const noexcept
    {
        if (formatEtc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!IsBitSet(formatEtc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (formatEtc.lindex != -1)
            return DV_E_LINDEX;

        return S_OK;
    }

    virtual void GetData(const FORMATETC&, STGMEDIUM&) const
    {
    }

    virtual void SetData(const FORMATETC&, STGMEDIUM&, bool /*bRelease*/)
    {
    }

    bool IsValid(const FORMATETC& formatEtc, const STGMEDIUM& stgmedium) const noexcept
    {
        return SUCCEEDED(Validate(formatEtc)) && formatEtc.tymed == stgmedium.tymed;
    }

protected:
    ClipboardFormatHandler(CLIPFORMAT clipFormat, bool bCanGetData, bool bCanSetData) noexcept
        : m_clipFormat(clipFormat),
          m_bCanGetData(bCanGetData),
          m_bCanSetData(bCanSetData)
    {
    }

    ClipboardFormatHandler(PCWSTR szFormat, bool bCanGetData, bool bCanSetData)
        : m_clipFormat(Win32::RegisterClipboardFormat(szFormat)),
          m_bCanGetData(bCanGetData),
          m_bCanSetData(bCanSetData)
    {
    }

private:
    CLIPFORMAT m_clipFormat;
    bool m_bCanGetData;
    bool m_bCanSetData;
};

} // namespace msf
