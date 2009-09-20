//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "ibrowserframeoptions.h"


namespace MSF
{

// Provide a more readable name for the MIDL generated enum.
typedef __MIDL_IBrowserFrameOptions_0001 EBrowserFrameOptions;


class ATL_NO_VTABLE IBrowserFrameOptionsImpl :
    public IBrowserFrameOptions
{
public:
    IBrowserFrameOptionsImpl(EBrowserFrameOptions browserframeoptions = BFO_NONE) :
        m_browserframeoptions(browserframeoptions)
    {
    }


    // IBrowserFrameOptions
    STDMETHOD(GetFrameOptions)(BROWSERFRAMEOPTIONS dwMask, _In_ BROWSERFRAMEOPTIONS* pdwOptions)
    {
        ATLTRACE2(atlTraceCOM, 0,
                  "IBrowserFrameOptionsImpl::GetFrameOptions (instance=%p, dwMask=%d, dwOptions=%d)\n",
                  this, dwMask, m_browserframeoptions);

        *pdwOptions = dwMask & m_browserframeoptions;

        return S_OK;
    }

protected:

    EBrowserFrameOptions m_browserframeoptions;
};

} // namespace MSF
