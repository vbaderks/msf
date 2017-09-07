//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

// Purpose: base class for items contained in a shellfolder.
//          The itembase doesn't own the actual item.
class ItemBase
{
public:
    USHORT GetDataSize() const noexcept
    {
        return m_pidl->mkid.cb - sizeof(short);
    }

    const BYTE* GetData() const noexcept
    {
        return m_pidl->mkid.abID;
    }

    LPCITEMIDLIST GetItemIdList() const
    {
        return m_pidl;
    }

protected:
    explicit ItemBase(LPCITEMIDLIST pidl) noexcept :
        m_pidl{ pidl }
    {
    }

private:

    LPCITEMIDLIST m_pidl;
};

} // end MSF namespace
