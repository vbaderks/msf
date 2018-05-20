//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include <Unknwn.h>
#include <ShlObj.h>

#undef  INTERFACE
#define INTERFACE  IShellFolderViewType
DECLARE_INTERFACE_IID_(IShellFolderViewType, IUnknown, "49422C1E-1C03-11d2-8DAB-0000F87A556C")
{
    // EnumViews:
    //   Returns an enumerator which will give out one pidl for every extended view.
    STDMETHOD(EnumViews)(ULONG grfFlags, __out IEnumIDList **ppenum) = 0;

    // GetDefaultViewName:
    //   Returns the name of the default view.  The names of the other views
    //   can be retrieved by calling GetDisplayNameOf.
    STDMETHOD(GetDefaultViewName)(DWORD  uFlags, __out PWSTR *ppwszName) PURE;
    STDMETHOD(GetViewTypeProperties)(PCUITEMID_CHILD pidl, __out DWORD *pdwFlags) = 0;

    // TranslateViewPidl:
    //   Attempts to take a pidl represented in one heirarchical representation of
    //   the Shell folder, and find it in a different representation.
    //   pidl should be relative to the root folder.
    //   Remember to ILFree ppidlOut
    STDMETHOD(TranslateViewPidl)(PCUIDLIST_RELATIVE pidl, PCUIDLIST_RELATIVE pidlView,
                                 __out PIDLIST_RELATIVE *ppidlOut) = 0;
};

#define SFVTFLAG_NOTIFY_CREATE  0x00000001
#define SFVTFLAG_NOTIFY_RESORT  0x00000002
