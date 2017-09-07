//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class __declspec(novtable) IBrowserFrameOptionsImpl :
    public IBrowserFrameOptions
{
public:
    // IBrowserFrameOptions
    HRESULT __stdcall GetFrameOptions(BROWSERFRAMEOPTIONS dwMask, _Out_ BROWSERFRAMEOPTIONS* pdwOptions) noexcept override
    {
        ATLTRACE2(atlTraceCOM, 0,
                  "IBrowserFrameOptionsImpl::GetFrameOptions (instance=%p, dwMask=%d, dwOptions=%d)\n",
                  this, dwMask, m_browserframeoptions);

        *pdwOptions = dwMask & m_browserframeoptions;

        return S_OK;
    }

protected:

    explicit IBrowserFrameOptionsImpl(BROWSERFRAMEOPTIONS browserframeoptions = BFO_NONE) :
        m_browserframeoptions(browserframeoptions)
    {
    }

    BROWSERFRAMEOPTIONS m_browserframeoptions;
};

} // namespace MSF
