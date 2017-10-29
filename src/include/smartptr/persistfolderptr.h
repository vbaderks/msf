//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "pidl.h"


_COM_SMARTPTR_TYPEDEF(IPersistFolder, __uuidof(IPersistFolder));


namespace msf
{

class IPersistFolderPtr : public ::IPersistFolderPtr
{
public:
    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IPersistFolderPtr(const _com_ptr_t<_OtherIID>& p) :
        ::IPersistFolderPtr(p)
    {
    }


    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IPersistFolderPtr(_InterfaceType* p) :
        ::IPersistFolderPtr(p)
    {
    }


    void Initialize(PCIDLIST_ABSOLUTE pidl)
    {
        RaiseExceptionIfFailed(GetInterfacePtr()->Initialize(pidl));
    }


    void Initialize(const wchar_t* szFilename)
    {
        ItemIDList pidl(szFilename);

        Initialize(pidl.GetAbsolute());
    }
};

} // end of msf namespace
