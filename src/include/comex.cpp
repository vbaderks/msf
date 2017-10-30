//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

class ComEx
{
public:
    // IContextMenu
    UINT QueryContextMenu(IContextMenu* p, HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
    {
        HRESULT hr = p->QueryContextMenu(hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags);
        RaiseExceptionIfFailed(hr);
        return static_cast<UINT>(HRESULT_CODE(hr));
    }

    void GetCommandString(IContextMenu* p, UINT_PTR idCmd, UINT uFlags, LPSTR pszName, UINT cchMax)
    {
        RaiseExceptionIfFailed(p->GetCommandString(idCmd, uFlags, nullptr, pszName, cchMax));
    }

    void GetCommandString(IContextMenu* p, UINT_PTR idCmd, UINT uFlags, LPWSTR wszName, UINT cchMax)
    {
        RaiseExceptionIfFailed(p->GetCommandString(idCmd, uFlags, nullptr, reinterpret_cast<char*>(wszName), cchMax));
    }
    

};


}