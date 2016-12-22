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
    IBrowserFrameOptionsImpl(BROWSERFRAMEOPTIONS browserframeoptions = BFO_NONE) :
        m_browserframeoptions(browserframeoptions)
    {
    }


    // IBrowserFrameOptions
    STDMETHOD(GetFrameOptions)(_In_ BROWSERFRAMEOPTIONS dwMask, _Out_ BROWSERFRAMEOPTIONS* pdwOptions)
    {
        ATLTRACE2(atlTraceCOM, 0,
                  "IBrowserFrameOptionsImpl::GetFrameOptions (instance=%p, dwMask=%d, dwOptions=%d)\n",
                  this, dwMask, m_browserframeoptions);

        *pdwOptions = dwMask & m_browserframeoptions;

        return S_OK;
    }

protected:

    BROWSERFRAMEOPTIONS m_browserframeoptions;
};

} // namespace MSF
