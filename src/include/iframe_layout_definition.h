//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

// INTERFACE: IFrameLayoutDefinition
// IFrameLayoutDefinition  is an undocumented interface introduced in Windows Vista. The definition below is based on info from Geoff Chappell.

struct __declspec(uuid("176C11B1-4302-4164-8430-D5A9F0EEACDB")) IFrameLayoutDefinition : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetLayoutDefinition (PCIDLIST_ABSOLUTE, HWND, IFrameLayoutDefinition **) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPossibleRedirectTarget (REFCLSID, INT *, PIDLIST_ABSOLUTE *) = 0;

    IFrameLayoutDefinition(const IFrameLayoutDefinition&) = delete;
    IFrameLayoutDefinition(IFrameLayoutDefinition&&) = delete;
    IFrameLayoutDefinition& operator=(const IFrameLayoutDefinition&) = delete;
    IFrameLayoutDefinition& operator=(IFrameLayoutDefinition&&) = delete;

protected:
    ~IFrameLayoutDefinition() = default;
};
