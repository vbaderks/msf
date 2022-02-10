//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

namespace msf
{

/// <purpose>Can hold a COM allocated string and will release it when it goes out of scope.</purpose>
class OleString final
{
public:
    OleString() = default;
    OleString(const OleString&) = delete;
    OleString(OleString&&) = delete;
    OleString& operator=(const OleString&) = delete;
    OleString& operator=(OleString&&) = delete;

    [[nodiscard]] static wchar_t* Dup(PCWSTR pszSrc)
    {
        wchar_t* pwz;
        RaiseExceptionIfFailed(SHStrDup(pszSrc, &pwz));
        return pwz;
    }

    ~OleString()
    {
        CoTaskMemFree(m_string);
    }

    // ReSharper disable once CppNonExplicitConversionOperator
    operator LPCOLESTR() const noexcept
    {
        return m_string;
    }

    [[nodiscard]] LPOLESTR* GetAddress() noexcept
    {
        ATLASSERT(!m_string); // "instance already owns a OLE string"
        return &m_string;
    }

private:
    LPOLESTR m_string{};
};

} // end namespace
