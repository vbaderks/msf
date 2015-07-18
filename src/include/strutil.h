//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

/// <summary>Converts the numeric input value to its equivalent string representation.</summary>
inline ATL::CString ToString(unsigned int uiValue)
{
    ATL::CString str;
    str.Format(L"%u", uiValue);
    return str;
}

/// <summary>Converts the numeric input value to its equivalent string representation.</summary>
inline ATL::CStringW ToStringW(unsigned int uiValue)
{
    ATL::CStringW str;
    str.Format(L"%u", uiValue);
    return str;
}

/// <summary>Load a resource string.</summary>
inline ATL::CString LoadString(UINT nID)
{
    ATL::CString str;
    ATLVERIFY(str.LoadString(nID));
    return str;
}

/// <summary>Load a resource string.</summary>
inline int CompareString(_In_ LCID Locale, _In_ DWORD dwCmpFlags, LPCTSTR lpString1, LPCTSTR lpString2)
{
    return ::CompareString(Locale, dwCmpFlags, lpString1, -1, lpString2, -1);
}

#ifndef _UNICODE
/// <summary>Load a Unicode resource string.</summary>
inline ATL::CStringW LoadStringW(UINT nID)
{
    ATL::CString str;
    ATLVERIFY(str.LoadString(nID));
    return CStringW(CA2W(str));
}
#endif

} // end MSF namespace
