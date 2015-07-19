//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace MSF
{

template <typename T>
class CGlobalLock
{
public:
    CGlobalLock() : m_p(nullptr), m_hMem(nullptr)
    {
    }


    CGlobalLock(HGLOBAL hMem) : m_p(GlobalLockThrow(hMem)), m_hMem(hMem)
    {
    }


    ~CGlobalLock()
    {
        Dispose();
    }


    void Attach(HGLOBAL hMem)
    {
        void* p = GlobalLockThrow(hMem);
        
        Dispose();

        m_p = p;
        m_hMem = hMem;
    }


    void Dispose() MSF_NOEXCEPT
    {
        if (m_hMem)
        {
            BOOL bResult = GlobalUnlock(m_hMem);
            ATLASSERT(bResult || GetLastError() == NO_ERROR); (bResult);
            m_hMem = nullptr;
            m_p = nullptr;
        }
    }


    T* get() const MSF_NOEXCEPT
    {
        return reinterpret_cast<T*>(m_p);
    }

private:

    static void* GlobalLockThrow(HGLOBAL hMem)
    {
        if (!hMem)
            return nullptr;

        void* p = GlobalLock(hMem);
        RaiseLastErrorExceptionIf(!p);
        return p;
    }

    CGlobalLock& operator=(const CGlobalLock&) MSF_NOEXCEPT; // not implemented by design.

    // Member variables
    void* m_p;
    HGLOBAL m_hMem;
};

} // namespace MSF
