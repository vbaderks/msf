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
inline CString ToString(unsigned int uiValue)
{
    CString str;
    str.Format(_T("%d"), uiValue);
    return str;
}

/// <summary>Converts the numeric input value to its equivalent string representation.</summary>
inline CStringW ToStringW(unsigned int uiValue)
{
    CStringW str;
    str.Format(L"%d", uiValue);
    return str;
}

/// <summary>Load a resource string.</summary>
inline CString LoadString(UINT nID)
{
    CString str;
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
inline CStringW LoadStringW(UINT nID)
{
    CString str;
    ATLVERIFY(str.LoadString(nID));
    return CStringW(CA2W(str));
}
#endif

} // end MSF namespace
