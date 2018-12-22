//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

namespace msf
{

class __declspec(novtable) IQueryInfoImpl :
    public IQueryInfo
{
public:
    IQueryInfoImpl(const IQueryInfoImpl &) = delete;
    IQueryInfoImpl(IQueryInfoImpl&&) = delete;
    IQueryInfoImpl & operator=(const IQueryInfoImpl &) = delete;
    IQueryInfoImpl& operator=(IQueryInfoImpl&&) = delete;

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
            ATLTRACE(L"IQueryInfoImpl::GetInfoTip (instance=%p, (dwFlags=%d)\n", this, dwFlags);
            return SHStrDup(GetInfoTip(dwFlags), ppwszTip);
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

protected:
    IQueryInfoImpl() noexcept
    {
        ATLTRACE(L"IQueryInfoImpl::IQueryInfoImpl (instance=%p)\n", this);
    }

    ~IQueryInfoImpl()
    {
        ATLTRACE(L"IQueryInfoImpl::~IQueryInfoImpl (instance=%p)\n", this);
    }

    virtual PCWSTR GetInfoTip(DWORD dwFlags) = 0;
};

} // namespace msf
