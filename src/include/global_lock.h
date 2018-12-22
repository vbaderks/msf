//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "msf_base.h"

namespace msf {
namespace util {

template <typename T>
class GlobalLock final
{
public:
    GlobalLock() = default;

    explicit GlobalLock(HGLOBAL memory)
        : m_p(GlobalLockThrow(memory)), m_memory(memory)
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

    void Attach(HGLOBAL memory)
    {
        void* p = GlobalLockThrow(memory);

        Dispose();

        m_p = p;
        m_memory = memory;
    }

    void Dispose() noexcept
    {
        if (m_memory)
        {
            const BOOL bResult = GlobalUnlock(m_memory);
            ATLASSERT(bResult || GetLastError() == NO_ERROR);
            UNREFERENCED_PARAMETER(bResult);
            m_memory = nullptr;
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
    static void* GlobalLockThrow(HGLOBAL memory)
    {
        if (!memory)
            return nullptr;

        void* p = ::GlobalLock(memory);
        RaiseLastErrorExceptionIf(!p);
        return p;
    }

    // Member variables
    void* m_p{};
    HGLOBAL m_memory{};
};

}
}
