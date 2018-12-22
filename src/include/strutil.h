//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>

namespace msf {

/// <summary>Load a resource string.</summary>
inline std::wstring LoadResourceString(UINT id)
{
    ATL::CString str;
    ATLVERIFY(str.LoadString(id));
    return str.GetString();
}

inline std::wstring FormatResourceMessage(UINT messageID, ...)
{
    va_list args;

#pragma warning(push)
#pragma warning(disable : 26492) // Don't use const_cast to cast away const (type.3).
    va_start(args, messageID);
#pragma warning(pop)

    PWSTR lpMsgBuf{};
    const auto size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
        reinterpret_cast<HMODULE>(&__ImageBase),
        messageID,
        0,
        reinterpret_cast<PWSTR>(&lpMsgBuf),
        0, &args);
    if (!size)
        RaiseLastErrorException();

    std::wstring result(lpMsgBuf, lpMsgBuf + size);
    HeapFree(GetProcessHeap(), 0, lpMsgBuf);
    return result;
}

inline int CompareString(_In_ LCID locale, _In_ DWORD dwCmpFlags, PCWSTR lpString1, PCWSTR lpString2) noexcept
{
    return ::CompareString(locale, dwCmpFlags, lpString1, -1, lpString2, -1);
}

inline void left_trim(std::wstring& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) noexcept { return !std::isspace(ch); }));
}

inline void right_trim(std::wstring& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) noexcept { return !std::isspace(ch); }).base(), s.end());
}

inline void trim(std::wstring& s)
{
    left_trim(s);
    right_trim(s);
}

} // end msf namespace
