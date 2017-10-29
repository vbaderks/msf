//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <locale>
#include <algorithm>
#include <functional>
#include <cctype>

namespace msf
{

/// <summary>Converts the numeric input value to its equivalent string representation.</summary>
inline std::wstring ToString(unsigned int uiValue)
{
    ATL::CString str;
    str.Format(L"%u", uiValue);
    return str.GetString(); // TODO
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

inline std::wstring LoadResourceString(UINT nID)
{
    auto s = LoadString(nID);
    return s.GetString();
}

inline std::wstring FormatResourceMessage(UINT messageID, ...)
{
    va_list args;
    va_start(args, messageID);

    LPWSTR lpMsgBuf = nullptr;
    auto size = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
        reinterpret_cast<HMODULE>(&__ImageBase),
        messageID,
        0,
        reinterpret_cast<LPWSTR>(&lpMsgBuf),
        0, &args);
    if (!size)
        RaiseLastErrorException();

    std::wstring result(lpMsgBuf, lpMsgBuf + size);
    HeapFree(GetProcessHeap(), 0, lpMsgBuf);
    return result;
}

inline int CompareString(_In_ LCID Locale, _In_ DWORD dwCmpFlags, LPCTSTR lpString1, LPCTSTR lpString2)
{
    return ::CompareString(Locale, dwCmpFlags, lpString1, -1, lpString2, -1);
}

// trim from start (in place)
inline void ltrim(std::wstring &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(std::wstring &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::wstring &s)
{
    ltrim(s);
    rtrim(s);
}


} // end msf namespace
