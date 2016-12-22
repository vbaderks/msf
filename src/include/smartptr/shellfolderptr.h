//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"
#include "persistfolderptr.h"


namespace MSF
{

class IShellFolderPtr : public ::IShellFolderPtr
{
public:
    IShellFolderPtr() :
        ::IShellFolderPtr()
    {
    }


    explicit IShellFolderPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_INPROC_SERVER) :
        ::IShellFolderPtr(clsid, pOuter, dwClsContext)
    {
    }


    IShellFolderPtr(const CLSID& clsid, const wchar_t* szFilename) :
        ::IShellFolderPtr(clsid, nullptr, CLSCTX_INPROC_SERVER)
    {
        Initialize(szFilename);
    }


    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IShellFolderPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IShellFolderPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IShellFolderPtr(_InterfaceType* p) :
        ::IShellFolderPtr(p)
    {
    }


    void Initialize(const wchar_t* szFilename)
    {
        MSF::IPersistFolderPtr persistfolder(this);

        persistfolder.Initialize(szFilename);
    }


    IEnumIDListPtr EnumObjects(SHCONTF grfFlags, HWND hwnd = nullptr)
    {
        IEnumIDListPtr enumidlist;
        RaiseExceptionIfFailed(GetInterfacePtr()->EnumObjects(hwnd, grfFlags, &enumidlist));

        return enumidlist;
    }


    IUnknownPtr CreateViewObject(REFIID riid, HWND hwndOwner = nullptr)
    {
        IUnknownPtr unknown;

        RaiseExceptionIfFailed(GetInterfacePtr()->CreateViewObject(hwndOwner, riid,
            reinterpret_cast<void**>(&(unknown.GetInterfacePtr()))));

        return unknown;
    }


    void GetAttributesOf(LPCITEMIDLIST apidl, SFGAOF* rgfInOut)
    {
        GetAttributesOf(1, &apidl, rgfInOut);
    }


    void GetAttributesOf(UINT cidl, LPCITEMIDLIST* apidl, SFGAOF* rgfInOut)
    {
        RaiseExceptionIfFailed(GetInterfacePtr()->GetAttributesOf(cidl, apidl, rgfInOut));
    }

    class CStrRet : public STRRET
    {
        CStrRet()
        {
            uType = STRRET_CSTR;
        }

        ~CStrRet()
        {
        }
    };


    CString GetDisplayNameOf(LPITEMIDLIST pidl, DWORD uFlags)
    {
        STRRET name;

        RaiseExceptionIfFailed(GetInterfacePtr()->GetDisplayNameOf(pidl, uFlags, &name));
    }

};

} // end of MSF namespace
