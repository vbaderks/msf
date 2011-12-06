//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "shelluuids.h"

_COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu));

namespace MSF
{

class IContextMenuPtr :
    public ::IContextMenuPtr
{
public:
    explicit IContextMenuPtr(const CLSID& clsid, IUnknown* pOuter = NULL, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IContextMenuPtr(clsid, pOuter, dwClsContext)
    {
        // Note: base class will not throw for E_NOINTERFACE.
        RaiseExceptionIf(GetInterfacePtr() == NULL, E_NOINTERFACE);
    }

    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IContextMenuPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IContextMenuPtr(p)
    {
    }

    void CreateInstance(const CLSID& rclsid)
    {
        RaiseExceptionIfFailed(__super::CreateInstance(rclsid));
    }

    UINT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
    {
        HRESULT hr = GetInterfacePtr()->QueryContextMenu(hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags);
        RaiseExceptionIfFailed(hr);

        return static_cast<UINT>(HRESULT_CODE(hr));
    }

    void GetCommandString(UINT_PTR idCmd, UINT uFlags, LPSTR pszName, UINT cchMax)
    {
        RaiseExceptionIfFailed(
            GetInterfacePtr()->GetCommandString(idCmd, uFlags, 0, pszName, cchMax));
    }

    void GetCommandString(UINT_PTR idCmd, UINT uFlags, LPWSTR wszName, UINT cchMax)
    {
        RaiseExceptionIfFailed(
            GetInterfacePtr()->GetCommandString(idCmd, uFlags, 0, reinterpret_cast<char*>(wszName), cchMax));
    }
};

} // end MSF namespace
