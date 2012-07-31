//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "catchhandler.h"

namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IEnumIDListImpl :
    public IEnumIDList
{
public:
    IEnumIDListImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, _T("IEnumIDListImpl::IEnumIDListImpl (instance=%p)\n"), this);
    }

    ~IEnumIDListImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, _T("IEnumIDListImpl::~IEnumIDListImpl (instance=%p)\n"), this);
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
                if (m_ppidl != NULL)
                {
                    for (ULONG i = 0; i < m_nCount; ++i)
                    {
                        CoTaskMemFree(m_ppidl[i]);
                    }
                }
            }

            void push_back(LPITEMIDLIST pidl) throw()
            {
                m_ppidl[m_nCount] = pidl;
                ++m_nCount;
            }

            ULONG size() const throw()
            {
                return m_nCount;
            }

            void release() throw()
            {
                m_ppidl = NULL;
            }

        private:
            ULONG         m_nCount;
            LPITEMIDLIST* m_ppidl;
    };

    // IEnumIDList
    STDMETHOD(Next)(ULONG celt, _Out_writes_to_(celt, *pceltFetched) LPITEMIDLIST* ppidl, _Out_opt_ ULONG* pceltFetched)
    {
        try
        {
            if (pceltFetched == NULL && celt != 1)
                return E_INVALIDARG;

            CItemIdListVector itemidlistvector(ppidl);

            for (ULONG i = 0; i < celt; ++i)
            {
                LPITEMIDLIST pidl = static_cast<T*>(this)->GetNextItem();
                if (pidl == NULL)
                    break; // No more items.

                itemidlistvector.push_back(pidl);
            }

            if (pceltFetched != NULL)
            {
                *pceltFetched = itemidlistvector.size();
            }

            itemidlistvector.release();

            return celt == itemidlistvector.size() ? S_OK : S_FALSE;
        }
        MSF_COM_CATCH_HANDLER()
    }

    STDMETHOD(Skip)(ULONG /*celt*/)
    {
        // Note: function not used by explorer \ system folder view.
        MSF_TRACENOTIMPL(_T("IEnumIDListImpl::Skip"));
    }

    STDMETHOD(Reset)()
    {
        // Note: function not used by explorer \ system folder view.
        MSF_TRACENOTIMPL(_T("IEnumIDListImpl::Reset"));
    }

    STDMETHOD(Clone)(__RPC__deref_out_opt IEnumIDList** /*ppenum*/)
    {
        // Note: function not used by explorer \ system folder view.
        MSF_TRACENOTIMPL(_T("IEnumIDListImpl::Clone"));
    }
};

} // namespace MSF
