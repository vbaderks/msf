//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#define MSF_ARRAY_SIZE(t) (sizeof(t) / sizeof(t[0]))


namespace MSF
{

constexpr inline bool ToBool(BOOL b)
{
    return b != FALSE;
}


bool ToBool(bool b) = delete; // not defined by design. Detects incorrect usage.


__declspec(noreturn) inline void RaiseException(HRESULT hr = E_FAIL)
{
    _com_raise_error(hr);
}


inline void RaiseExceptionIfFailed(HRESULT hr)
{
    if (FAILED(hr))
        RaiseException(hr);
}


inline void RaiseExceptionIf(bool bTest, HRESULT hr = E_FAIL)
{
    if (bTest)
        RaiseException(hr);
}


inline void RaiseLastErrorExceptionIf(bool bTest)
{
    RaiseExceptionIf(bTest, HRESULT_FROM_WIN32(GetLastError()));
}


inline void RaiseLastErrorExceptionIf(BOOL bTest)
{
    RaiseLastErrorExceptionIf(ToBool(bTest));
}


constexpr inline bool IsBitSet(unsigned long lValue, unsigned long lTestMask)
{
    return (lValue & lTestMask) != 0;
}

} // end of MSF namespace
