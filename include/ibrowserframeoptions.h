//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

// The Win32 Platform SDK of February 2003 defines the IBrowserFrameOptions interface
// This definition is removed from the Platform SDK included with Visual Studio .NET 2003
//
#ifndef __IBrowserFrameOptions_INTERFACE_DEFINED__
#define __IBrowserFrameOptions_INTERFACE_DEFINED__

enum __MIDL_IBrowserFrameOptions_0001
{
    BFO_NONE                             = 0,
    BFO_BROWSER_PERSIST_SETTINGS         = 0x1,
    BFO_RENAME_FOLDER_OPTIONS_TOINTERNET = 0x2,
    BFO_BOTH_OPTIONS                     = 0x4,
    BIF_PREFER_INTERNET_SHORTCUT         = 0x8,
    BFO_BROWSE_NO_IN_NEW_PROCESS         = 0x10,
    BFO_ENABLE_HYPERLINK_TRACKING        = 0x20,
    BFO_USE_IE_OFFLINE_SUPPORT           = 0x40,
    BFO_SUBSTITUE_INTERNET_START_PAGE    = 0x80,
    BFO_USE_IE_LOGOBANDING               = 0x100,
    BFO_ADD_IE_TOCAPTIONBAR              = 0x200,
    BFO_USE_DIALUP_REF                   = 0x400,
    BFO_USE_IE_TOOLBAR                   = 0x800,
    BFO_NO_PARENT_FOLDER_SUPPORT         = 0x1000,
    BFO_NO_REOPEN_NEXT_RESTART           = 0x2000,
    BFO_GO_HOME_PAGE                     = 0x4000,
    BFO_PREFER_IEPROCESS                 = 0x8000,
    BFO_SHOW_NAVIGATION_CANCELLED        = 0x10000,
    BFO_QUERY_ALL                        = 0xffffffff
};
typedef DWORD BROWSERFRAMEOPTIONS;

// INTERFACE: IBrowserFrameOptions
//
// This interface was implemented so a browser or host can ask a ShellView/ShelNameSpace what
// kind of 'Behavior' is appropriate for that view.
//
//    IBrowserFrameOptions::GetBrowserOptions()
//       dwMask is the logical OR of bits to look for.  pdwOptions is not optional and
//       it's return value will always equal or will be a subset of dwMask.
//       If the function succeeds, the return value must be S_OK and pdwOptions needs to be filled in.
//       If the function fails, pdwOptions needs to be filled in with BFO_NONE.

interface IBrowserFrameOptions : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetFrameOptions(
        _In_  BROWSERFRAMEOPTIONS  dwMask,
        _Out_ BROWSERFRAMEOPTIONS* pdwOptions) = 0;
};

// The Win32 SDK header files don't attach the GUID to the interface.
struct __declspec(uuid("10DF43C8-1DBE-11d3-8B34-006097DF5BD4")) IBrowserFrameOptions;

#endif // not __IBrowserFrameOptions_INTERFACE_DEFINED__
