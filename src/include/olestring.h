//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{
/// <purpose>Can hold and COM allocated string and will release it when it goes out of scope.</purpose>
class COleString
{
public:
    static wchar_t* Dup(LPCTSTR pszSrc)
    {
        wchar_t* pwz;
        RaiseExceptionIfFailed(SHStrDup(pszSrc, &pwz));
        return pwz;
    }

    COleString() noexcept : m_olestrCLSID(nullptr)
    {
    }

    ~COleString()
    {
        CoTaskMemFree(m_olestrCLSID);
    }

    COleString(const COleString&) = delete;

    COleString& operator=(const COleString&) = delete;

    operator LPOLESTR() const noexcept
    {
        return m_olestrCLSID;
    }

    LPOLESTR* GetAddress() noexcept
    {
        ATLASSERT(!m_olestrCLSID || !"instance already owns a olestring"); 
        return &m_olestrCLSID;
    }

private:

    LPOLESTR m_olestrCLSID;
};

} // end namespace
