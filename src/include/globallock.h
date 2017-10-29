//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf {
namespace util {

template <typename T>
class GlobalLock
{
public:
    GlobalLock() : m_p(nullptr), m_hMem(nullptr)
    {
    }

    explicit GlobalLock(HGLOBAL hMem) : m_p(GlobalLockThrow(hMem)), m_hMem(hMem)
    {
    }

    ~GlobalLock()
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

    void Dispose() noexcept
    {
        if (m_hMem)
        {
            BOOL bResult = GlobalUnlock(m_hMem);
            ATLASSERT(bResult || GetLastError() == NO_ERROR);
            UNREFERENCED_PARAMETER(bResult);
            m_hMem = nullptr;
            m_p = nullptr;
        }
    }

    T* get() const noexcept
    {
        return reinterpret_cast<T*>(m_p);
    }

private:

    static void* GlobalLockThrow(HGLOBAL hMem)
    {
        if (!hMem)
            return nullptr;

        void* p = ::GlobalLock(hMem);
        RaiseLastErrorExceptionIf(!p);
        return p;
    }

    GlobalLock& operator=(const GlobalLock&) = delete;

    // Member variables
    void* m_p;
    HGLOBAL m_hMem;
};

}
}
