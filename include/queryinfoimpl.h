//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IQueryInfoImpl :
    public IQueryInfo
{
public:
    IQueryInfoImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IQueryInfoImpl::IQueryInfoImpl (instance=%p)\n", this);
    }

    ~IQueryInfoImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IQueryInfoImpl::~IQueryInfoImpl (instance=%p)\n", this);
    }

    // IQueryInfo
    STDMETHOD(GetInfoFlags)(_Out_ DWORD* /* pdwFlags */)
    {
        // The Vista shell will call GetInfoFlags. The msdn docs are incomplete about the use of this function.
        // Possible return values are:
        // - QIF_CACHED 
        // - QIF_DONTEXPANDFOLDER

        MSF_TRACENOTIMPL(L"IQueryInfoImpl::GetInfoFlags");
    }

    STDMETHOD(GetInfoTip)(DWORD dwFlags, _Outptr_ WCHAR** ppwszTip)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, L"IQueryInfoImpl::GetInfoTip (dwFlags=%d)\n", dwFlags);
            return SHStrDup(static_cast<T*>(this)->GetInfoTip(dwFlags), ppwszTip);
        }
        MSF_COM_CATCH_HANDLER()
    }
};

} // namespace MSF
