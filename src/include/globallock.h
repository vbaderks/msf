//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "msfbase.h"

namespace msf {
namespace util {

template <typename T>
class GlobalLock final
{
public:
    GlobalLock() = default;

    explicit GlobalLock(HGLOBAL hMem) : m_p(GlobalLockThrow(hMem)), m_hMem(hMem)
    {
    }

    ~GlobalLock()
    {
        Dispose();
    }

    GlobalLock(const GlobalLock&) = delete;
    GlobalLock(GlobalLock&&) = delete;
    GlobalLock& operator=(const GlobalLock&) = delete;
    GlobalLock& operator=(GlobalLock&&) = delete;

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
            const BOOL bResult = GlobalUnlock(m_hMem);
            ATLASSERT(bResult || GetLastError() == NO_ERROR);
            UNREFERENCED_PARAMETER(bResult);
            m_hMem = nullptr;
            m_p = nullptr;
        }
    }

#pragma warning(push)
#pragma warning(disable: 26473) // Don't cast between pointer types where the source type and the target type are the same
    T* get() const noexcept
    {
        return static_cast<T*>(m_p);
    }
#pragma warning(pop)

private:

    static void* GlobalLockThrow(HGLOBAL hMem)
    {
        if (!hMem)
            return nullptr;

        void* p = ::GlobalLock(hMem);
        RaiseLastErrorExceptionIf(!p);
        return p;
    }

    // Member variables
    void* m_p{};
    HGLOBAL m_hMem{};
};

}
}
