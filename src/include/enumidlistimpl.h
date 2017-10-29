//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"

namespace msf
{

template <typename T>
class __declspec(novtable) IEnumIDListImpl :
    public IEnumIDList
{
public:
    IEnumIDListImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IEnumIDListImpl::IEnumIDListImpl (instance=%p)\n", this);
    }


    ~IEnumIDListImpl()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"IEnumIDListImpl::~IEnumIDListImpl (instance=%p)\n", this);
    }


    class CItemIdListVector
    {
        public:
            explicit CItemIdListVector(LPITEMIDLIST* ppidl) :
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
    HRESULT __stdcall Next(ULONG celt, _Out_writes_to_(celt, *pceltFetched) PITEMID_CHILD* ppidl, _Out_opt_ ULONG* pceltFetched) noexcept override
    {
        try
        {
            if (!pceltFetched && celt != 1)
                return E_INVALIDARG;

            CItemIdListVector itemidlistvector(reinterpret_cast<LPITEMIDLIST*>(ppidl));

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
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Skip(ULONG /*celt*/) noexcept override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Skip");
    }

    HRESULT __stdcall Reset() noexcept override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Reset");
    }

    HRESULT __stdcall Clone(__RPC__deref_out_opt IEnumIDList** /*ppenum*/) override
    {
        // Note: function not used by explorer \ system folder view.
        ATLTRACENOTIMPL(L"IEnumIDListImpl::Clone");
    }
};

} // namespace msf
