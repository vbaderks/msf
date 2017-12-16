//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include <stdexcept>


namespace msf
{

[[noreturn]] inline void RaiseException(HRESULT hr = E_FAIL)
{
    _com_raise_error(hr);
}

[[noreturn]] inline void RaiseLastErrorException()
{
    RaiseException(HRESULT_FROM_WIN32(GetLastError()));
}

inline void RaiseExceptionIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        RaiseException(hr);
    }
}

inline void RaiseExceptionIf(bool bTest, HRESULT hr = E_FAIL)
{
    if (bTest)
    {
        RaiseException(hr);
    }
}

inline void RaiseLastErrorExceptionIf(bool bTest)
{
    RaiseExceptionIf(bTest, HRESULT_FROM_WIN32(GetLastError()));
}

constexpr bool IsBitSet(unsigned long lValue, unsigned long lTestMask)
{
    return (lValue & lTestMask) != 0;
}

inline __declspec(noinline) HRESULT ExceptionToHResult() noexcept
{
    try
    {
        throw;
    }
    catch (const _com_error& e)
    {
        return e.Error();
    }
    catch (const std::bad_alloc &)
    {
        return E_OUTOFMEMORY;
    }
    catch (const std::out_of_range &)
    {
        return E_BOUNDS;
    }
    catch (const std::invalid_argument &)
    {
        return E_INVALIDARG;
    }
    catch (const std::exception &)
    {
        return E_FAIL;
    }
}


} // end of msf namespace
