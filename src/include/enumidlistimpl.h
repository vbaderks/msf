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
class ATL_NO_VTABLE IEnumIDListImpl :
    public IEnumIDList
{
public:
    IEnumIDListImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IEnumIDListImpl::IEnumIDListImpl (instance=%p)\n", this);
    }


    ~IEnumIDListImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IEnumIDListImpl::~IEnumIDListImpl (instance=%p)\n", this);
    }


    class CItemIdListVector
    {
        public:
            CItemIdListVector(LPITEMIDLIST* ppidl) :
                m_nCount(0), m_ppidl(ppidl)
            {
            }


            ~CItemIdListVector()
            {
                if (m_ppidl)
                {
                    for (ULONG i = 0; i < m_nCount; ++i)
                    {
                        CoTaskMemFree(m_ppidl[i]);
                    }
                }
            }


            void push_back(LPITEMIDLIST pidl) noexcept
            {
                m_ppidl[m_nCount] = pidl;
                ++m_nCount;
            }


            ULONG size() const noexcept
            {
                return m_nCount;
            }


            void release() noexcept
            {
                m_ppidl = nullptr;
            }

        private:
            ULONG         m_nCount;
            LPITEMIDLIST* m_ppidl;
    };

    // IEnumIDList
    STDMETHOD(Next)(ULONG celt, _Out_writes_to_(celt, *pceltFetched) LPITEMIDLIST* ppidl, _Out_opt_ ULONG* pceltFetched) override
    {
        try
        {
            if (!pceltFetched && celt != 1)
                return E_INVALIDARG;

            CItemIdListVector itemidlistvector(ppidl);

            for (ULONG i = 0; i < celt; ++i)
            {
                LPITEMIDLIST pidl = static_cast<T*>(this)->GetNextItem();
                if (!pidl)
                    break; // No more items.

                itemidlistvector.push_back(pidl);
            }

            if (pceltFetched)
            {
                *pceltFetched = itemidlistvector.size();
            }

            itemidlistvector.release();

            return celt == itemidlistvector.size() ? S_OK : S_FALSE;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(Skip)(ULONG /*celt*/) override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Skip");
    }

    STDMETHOD(Reset)() override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Reset");
    }

    STDMETHOD(Clone)(__RPC__deref_out_opt IEnumIDList** /*ppenum*/) override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Clone");
    }
};

} // namespace MSF
