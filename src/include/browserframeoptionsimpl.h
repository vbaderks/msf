//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace MSF
{

class ATL_NO_VTABLE IBrowserFrameOptionsImpl :
    public IBrowserFrameOptions
{
public:
    // IBrowserFrameOptions
    STDMETHOD(GetFrameOptions)(_In_ BROWSERFRAMEOPTIONS dwMask, _Out_ BROWSERFRAMEOPTIONS* pdwOptions) override
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
