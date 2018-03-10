//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

// Purpose: Management class for pidls. A CPidl class is owner of the wrapped ITEMIDLIST.
class ItemIDList final
{
public:
    static PIDLIST_RELATIVE Clone(_In_ PCUIDLIST_RELATIVE pidlSrc)
    {
        ATLASSERT(pidlSrc && "Why clone a NULL pointer?");

        const PIDLIST_RELATIVE pidl = ILClone(pidlSrc);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);
        return pidl;
    }

    static PIDLIST_ABSOLUTE CloneFull(_In_ PCUIDLIST_ABSOLUTE pidlSrc)
    {
        ATLASSERT(pidlSrc && "Why clone a NULL pointer?");

        const PIDLIST_ABSOLUTE pidl = ILCloneFull(pidlSrc);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);
        return pidl;
    }

    static PIDLIST_ABSOLUTE Combine(_In_opt_ PCIDLIST_ABSOLUTE pidl1, _In_opt_ PCUIDLIST_RELATIVE pidl2)
    {
        PIDLIST_ABSOLUTE pidl = ILCombine(pidl1, pidl2);
        RaiseExceptionIf(!pidl && !(pidl1 == nullptr && pidl == nullptr), E_OUTOFMEMORY);
        return pidl;
    }

    static LPITEMIDLIST CreateFromPath(LPCTSTR pszPath)
    {
        const LPITEMIDLIST pidl = ILCreateFromPath(pszPath);
        RaiseExceptionIf(!pidl, E_OUTOFMEMORY);
        return pidl;
    }

    static PUIDLIST_RELATIVE CreateItemIdListWithTerminator(size_t sizeItem)
    {
        const size_t size = sizeof(short) + sizeItem;

        auto pidl = static_cast<PUIDLIST_RELATIVE>(CoTaskMemAlloc(size + sizeof(short)));
        if (!pidl)
            RaiseException(E_OUTOFMEMORY);

        LPSHITEMID pshitemid = &(pidl->mkid);
        pshitemid->cb = static_cast<USHORT>(size);

        PUIDLIST_RELATIVE pidlTerminator = ILGetNext(pidl);
        LPSHITEMID pItemIdTerminator = &(pidlTerminator->mkid);
        pItemIdTerminator->cb = 0;

        return pidl;
    }

    // Purpose: Small helper, returns nullptr also for the tail element.
    static PUIDLIST_RELATIVE GetNextItem(PCUIDLIST_RELATIVE pidl) noexcept
    {
        PUIDLIST_RELATIVE pidlNext = ILGetNext(pidl);
        if (pidlNext && pidlNext->mkid.cb == 0)
        {
            pidlNext = nullptr;
        }

        return pidlNext;
    }

    ItemIDList() noexcept : m_pidl(nullptr)
    {
    }

    explicit ItemIDList(PUIDLIST_RELATIVE itemIDList) noexcept :
        m_pidl(reinterpret_cast<LPITEMIDLIST>(itemIDList))
    {
    }

    ItemIDList(_In_opt_ PCIDLIST_ABSOLUTE pidl1, _In_opt_ PCUIDLIST_RELATIVE pidl2) :
        m_pidl(Combine(pidl1, pidl2))
    {
    }

    ItemIDList(const ItemIDList& pidl1, PCUIDLIST_RELATIVE pidl2) :
        m_pidl(Combine(pidl1.GetAbsolute(), pidl2))
    {
    }

    ItemIDList(const ItemIDList& pidl1, const ItemIDList& pidl2) :
        m_pidl(Combine(pidl1.GetAbsolute(), pidl2.GetRelative()))
    {
    }

    explicit ItemIDList(LPCTSTR pszPath) :
        m_pidl(CreateFromPath(pszPath))
    {
    }

    ~ItemIDList()
    {
        CoTaskMemFree(m_pidl);
    }

    ItemIDList(const ItemIDList&) = delete;
    ItemIDList(ItemIDList&&) = delete;
    ItemIDList& operator=(const ItemIDList&) = delete;
    ItemIDList& operator=(ItemIDList&&) = delete;

    void Attach(PUIDLIST_RELATIVE pidl) noexcept
    {
        CoTaskMemFree(m_pidl);
        m_pidl = pidl;
    }

#ifdef STRICT_TYPED_ITEMIDS
    void Attach(PIDLIST_ABSOLUTE pidl) noexcept
    {
        Attach(reinterpret_cast<PUIDLIST_RELATIVE>(pidl));
    }
#endif

    PUIDLIST_RELATIVE DetachRelative() noexcept
    {
        const auto pidl = reinterpret_cast<PUIDLIST_RELATIVE>(m_pidl);
        m_pidl = nullptr;
        return pidl;
    }

    PIDLIST_ABSOLUTE DetachAbsolute() noexcept
    {
        const auto pidl = reinterpret_cast<PIDLIST_ABSOLUTE>(m_pidl);
        m_pidl = nullptr;
        return pidl;
    }

    void CloneFrom(PCUIDLIST_RELATIVE pidl)
    {
        Attach(Clone(pidl));
    }

    PIDLIST_RELATIVE Clone() const
    {
        return Clone(GetRelative());
    }

    PIDLIST_ABSOLUTE CloneFull() const
    {
        return CloneFull(GetAbsolute());
    }

    void AppendID(const SHITEMID* pmkid)
    {
        PIDLIST_RELATIVE pidl = ILAppendID(GetRelative(), pmkid, true);
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
        return ILGetSize(GetRelative());
    }

    PIDLIST_RELATIVE GetRelative() const noexcept
    {
        return reinterpret_cast<PIDLIST_RELATIVE>(m_pidl);
    }

    PIDLIST_ABSOLUTE GetAbsolute() const noexcept
    {
        return reinterpret_cast<PIDLIST_ABSOLUTE>(m_pidl);
    }

    // Purpose: Address operator to be used for passing address to be used as an out-parameter.
    LPITEMIDLIST* operator&() noexcept
    {
        CoTaskMemFree(m_pidl);
        m_pidl = nullptr;
        return &m_pidl;
    }

private:
    LPITEMIDLIST m_pidl;
};

} // namespace msf
