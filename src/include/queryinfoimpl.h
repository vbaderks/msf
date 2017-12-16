//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

class __declspec(novtable) IQueryInfoImpl :
    public IQueryInfo
{
public:
    IQueryInfoImpl(const IQueryInfoImpl &) = delete;
    IQueryInfoImpl & operator=(const IQueryInfoImpl &) = delete;

    // IQueryInfo
    HRESULT __stdcall GetInfoFlags(_Out_ DWORD* /* pdwFlags */) noexcept override
    {
        // The Vista shell will call GetInfoFlags. The MSDN docs are incomplete about the use of this function.
        // Possible return values are:
        // - QIF_CACHED
        // - QIF_DONTEXPANDFOLDER

        ATLTRACENOTIMPL(L"IQueryInfoImpl::GetInfoFlags");
    }

    HRESULT __stdcall GetInfoTip(DWORD dwFlags, _Outptr_ WCHAR** ppwszTip) noexcept override
    {
        try
        {
            ATLTRACE2(ATL::atlTraceCOM, 0, L"IQueryInfoImpl::GetInfoTip (dwFlags=%d)\n", dwFlags);
            return SHStrDup(GetInfoTip(dwFlags).c_str(), ppwszTip);
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:

    IQueryInfoImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IQueryInfoImpl::IQueryInfoImpl (instance=%p)\n", this);
    }

    ~IQueryInfoImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IQueryInfoImpl::~IQueryInfoImpl (instance=%p)\n", this);
    }

    virtual std::wstring GetInfoTip(DWORD dwFlags) = 0;
};

} // namespace msf
