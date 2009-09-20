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

    COleString() throw() : m_olestrCLSID(NULL)
    {
    }

    ~COleString() throw()
    {
        CoTaskMemFree(m_olestrCLSID);
    }

    operator LPOLESTR() const throw()
    {
        return m_olestrCLSID;
    }

    LPOLESTR* GetAddress() throw()
    {
        ATLASSERT(m_olestrCLSID == NULL || !"instance already owns a olestring"); 
        return &m_olestrCLSID;
    }

private:
    COleString(const COleString&) throw();            // not implemented by design.
    COleString& operator=(const COleString&) throw(); // not implemented by design.

    LPOLESTR m_olestrCLSID;
};

} // end namespace
