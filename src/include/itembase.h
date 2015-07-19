//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "macros.h"

namespace MSF
{

// Purpose: base class for items contained in a shellfolder.
//          The itembase doesn't own the actual item.
class CItemBase
{
public:
    CItemBase(LPCITEMIDLIST pidl) MSF_NOEXCEPT :
        m_pidl(pidl)
    {
    }


    USHORT GetDataSize() const MSF_NOEXCEPT
    {
        return m_pidl->mkid.cb - sizeof(short);
    }


    const BYTE* GetData() const MSF_NOEXCEPT
    {
        return m_pidl->mkid.abID;
    }


    LPCITEMIDLIST GetItemIdList() const
    {
        return m_pidl;
    }

private:

    LPCITEMIDLIST m_pidl;
};

} // end MSF namespace
