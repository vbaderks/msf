//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"


namespace msf
{

struct ShellItemIdMember
{
    size_t      size;
    const void* pdata;
};


class CShellItemIdMember : public ShellItemIdMember
{
public:
    explicit CShellItemIdMember(const bool* pbValue)
    {
        size = sizeof *pbValue;
        pdata = pbValue;
    }


    explicit CShellItemIdMember(const unsigned int* pnValue)
    {
        size = sizeof(*pnValue);
        pdata = pnValue;
    }


    explicit CShellItemIdMember(PCWSTR wszValue)
    {
        size = (wcslen(wszValue) + 1) * sizeof(wchar_t);
        pdata = wszValue;
    }
};


class CShellItemId
{
public:
    CShellItemId(const ShellItemIdMember* members, unsigned int count, bool bAddEmptyId = true) noexcept
    {
        const size_t size = sizeof(short) + GetMembersSize(members, count);

        m_itemId = static_cast<SHITEMID*>(CoTaskMemAlloc(size + (bAddEmptyId ? sizeof(short) : 0)));
        if (!m_itemId)
            return;

        SetSize(size);

        // Copy members into allocated SHITEMID.
        BYTE* p = m_itemId->abID;
        for (unsigned int i = 0; i < count; ++i)
        {
            memcpy(p, members[i].pdata, members[i].size);
            p += members[i].size;
        }

        // Add optional terminating SHITEMID to make it easy to convert to a ITEMIDLIST.
        if (bAddEmptyId)
        {
            SHITEMID* pItemIdTerminating = reinterpret_cast<SHITEMID*>(p);
            pItemIdTerminating->cb = 0;
        }
    }


    ~CShellItemId()
    {
        CoTaskMemFree(m_itemId);
    }


    SHITEMID* Detach() noexcept
    {
        SHITEMID* pItemId = m_itemId;
        m_itemId = nullptr;
        return pItemId;
    }

private:

    void SetSize(size_t size) const noexcept
    {
        ATLASSERT(size <= USHRT_MAX && "size will be sliced!");
        m_itemId->cb = static_cast<USHORT>(size);
    }

    static size_t GetMembersSize(const ShellItemIdMember* members, unsigned int count) noexcept
    {
        size_t size = 0;

        for (unsigned int i = 0; i < count; ++i)
        {
            size += members[i].size;
        }

        return size;
    }


    SHITEMID* m_itemId;
};


class CShellItemIterator
{
public:
    explicit CShellItemIterator(const SHITEMID& itemid) noexcept : m_p(itemid.abID)
    {
#ifdef _DEBUG
        m_itemId = &itemid;
#endif
    }

    ~CShellItemIterator()
    {
#ifdef _DEBUG
        const size_t s = static_cast<size_t>(m_p - m_itemId->abID);
        ATLASSERT(s == (m_itemId->cb - sizeof(unsigned short)) && "not all items were retrieved!");
#endif
    }


    bool GetBool() noexcept
    {
        const bool b = *reinterpret_cast<const bool *>(m_p);
        m_p += sizeof(b);
        return b;
    }

    unsigned int GetUnsignedInt() noexcept
    {
        constunsigned int n = *reinterpret_cast<const unsigned int*>(m_p);
        m_p += sizeof(n);
        return n;
    }

    ATL::CString GetString() const
    {
        // Note: strings are always stored in Unicode to prevent ANSI/Unicode mismatches.
        return ATL::CString(reinterpret_cast<PCWSTR>(m_p));
    }

private:
    const BYTE* m_p;

#ifdef _DEBUG
    const SHITEMID* m_itemId;
#endif
};

} // namespace msf
