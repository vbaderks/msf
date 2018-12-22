//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace msf
{

class __declspec(novtable) IBrowserFrameOptionsImpl :
    public IBrowserFrameOptions
{
public:
    // IBrowserFrameOptions
    HRESULT __stdcall GetFrameOptions(BROWSERFRAMEOPTIONS mask, _Out_ BROWSERFRAMEOPTIONS* options) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0,
                  "IBrowserFrameOptionsImpl::GetFrameOptions (instance=%p, dwMask=%d, dwOptions=%d)\n",
                  this, mask, m_browserFrameOptions);

        *options = mask & m_browserFrameOptions;

        return S_OK;
    }

    IBrowserFrameOptionsImpl(const IBrowserFrameOptionsImpl&) = delete;
    IBrowserFrameOptionsImpl(IBrowserFrameOptionsImpl&&) = delete;
    IBrowserFrameOptionsImpl& operator=(const IBrowserFrameOptionsImpl&) = delete;
    IBrowserFrameOptionsImpl& operator=(IBrowserFrameOptionsImpl&&) = delete;

protected:
    explicit IBrowserFrameOptionsImpl(BROWSERFRAMEOPTIONS browserFrameOptions = BFO_NONE) noexcept :
        m_browserFrameOptions{browserFrameOptions}
    {
    }

    ~IBrowserFrameOptionsImpl() = default;

    BROWSERFRAMEOPTIONS m_browserFrameOptions;
};

} // namespace msf
