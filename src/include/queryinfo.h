//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "queryinfoimpl.h"


namespace MSF
{
class ATL_NO_VTABLE CQueryInfo :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IQueryInfoImpl
{
public:
    static CComPtr<IQueryInfo> CreateInstance(std::wstring infoTipText)
    {
        CComObject<CQueryInfo>* p;
        HRESULT hr = CComObject<CQueryInfo>::CreateInstance(&p);
        if (FAILED(hr))
            RaiseException(hr);

        p->m_infoTipText = infoTipText;
        return p;
    }

    DECLARE_NOT_AGGREGATABLE(CQueryInfo)

    BEGIN_COM_MAP(CQueryInfo)
        COM_INTERFACE_ENTRY(IQueryInfo)
    END_COM_MAP()

protected:

    std::wstring GetInfoTip(DWORD /* dwFlags */) override
    {
        return m_infoTipText;
    }

private:
    std::wstring m_infoTipText;
};

} // namespace MSF
