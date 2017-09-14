//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

// Purpose: Management class for pidls. A CPidl class is owner of the wrapped ITEMIDLIST.
class ItemIDList final
{
public:
    static LPITEMIDLIST Clone(_In_ LPCITEMIDLIST pidlSrc)
    {
        ATLASSERT(pidlSrc && "Why clone a NULL pointer?");

        LPITEMIDLIST pidl = ILClone(pidlSrc);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);
        return pidl;
    }

    static LPITEMIDLIST Combine(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
    {
        LPITEMIDLIST pidl = ILCombine(pidl1, pidl2);
        RaiseExceptionIf(!pidl && !(pidl1 == nullptr && pidl == nullptr), E_OUTOFMEMORY);
        return pidl;
    }

    static LPITEMIDLIST CreateFromPath(LPCTSTR pszPath)
    {
        LPITEMIDLIST pidl = ILCreateFromPath(pszPath);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);
        return pidl;
    }

    static LPITEMIDLIST CreateItemIdListWithTerminator(size_t sizeItem)
    {
        size_t size = sizeof(short) + sizeItem;

        LPITEMIDLIST pidl = static_cast<LPITEMIDLIST>(CoTaskMemAlloc(size + (sizeof(short))));
        if (!pidl)
            RaiseException(E_OUTOFMEMORY);

        LPSHITEMID pshitemid = &(pidl->mkid);
        pshitemid->cb = static_cast<USHORT>(size);

        LPITEMIDLIST pidlTerminator = ILGetNext(pidl);
        LPSHITEMID pItemIdTerminator = &(pidlTerminator->mkid);
        pItemIdTerminator->cb = 0;

        return pidl;
    }

    // Purpose: Small helper, returns nullptr also for the tail element.
    static LPCITEMIDLIST GetNextItem(LPCITEMIDLIST pidl)
    {
        LPCITEMIDLIST pidlNext = ILGetNext(pidl);
        if (pidlNext && pidlNext->mkid.cb == 0)
        {
            pidlNext = nullptr;
        }

        return pidlNext;
    }

    ItemIDList() noexcept : m_pidl(nullptr)
    {
    }

    explicit ItemIDList(LPITEMIDLIST itemIDList) noexcept : m_pidl(itemIDList)
    {
    }

    ItemIDList(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) :
        m_pidl(Combine(pidl1, pidl2))
    {
    }

    ItemIDList(const ItemIDList& pidl1, LPCITEMIDLIST pidl2) :
        m_pidl(Combine(pidl1.m_pidl, pidl2))
    {
    }

    explicit ItemIDList(const wchar_t* pszPath) :
        m_pidl(CreateFromPath(pszPath))
    {
    }

    ~ItemIDList()
    {
        ILFree(m_pidl);
    }

    void Attach(LPITEMIDLIST pidl) noexcept
    {
        ILFree(m_pidl);
        m_pidl = pidl;
    }

    LPITEMIDLIST Detach() noexcept
    {
        LPITEMIDLIST pidl = m_pidl;
        m_pidl = nullptr;
        return pidl;
    }

    void CloneFrom(LPCITEMIDLIST pidl)
    {
        Attach(Clone(pidl));
    }

    LPITEMIDLIST Clone() const
    {
        return Clone(m_pidl);
    }

    void AppendID(const SHITEMID* pmkid)
    {
        LPITEMIDLIST pidl = ILAppendID(m_pidl, pmkid, TRUE);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);

        m_pidl = pidl;
    }

    LPITEMIDLIST get() const noexcept
    {
        return m_pidl;
    }

    operator LPCITEMIDLIST() const noexcept
    {
        return get();
    }

    LPCITEMIDLIST operator->() const noexcept
    {
        return get();
    }

    UINT GetSize() const noexcept
    {
        return ILGetSize(m_pidl);
    }

    // Purpose: Address operator to be used for passing address to be used as an out-parameter.
    LPITEMIDLIST* operator&() noexcept
    {
        Attach(nullptr);
        return &m_pidl;
    }

private:
    LPITEMIDLIST m_pidl;
};

} // namespace MSF
