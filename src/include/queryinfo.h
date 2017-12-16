﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "queryinfoimpl.h"


namespace msf
{

class __declspec(novtable) QueryInfo :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public IQueryInfoImpl
{
public:
    static ATL::CComPtr<IQueryInfo> CreateInstance(std::wstring infoTipText)
    {
        ATL::CComObject<QueryInfo>* p;
        const HRESULT hr = ATL::CComObject<QueryInfo>::CreateInstance(&p);
        if (FAILED(hr))
            RaiseException(hr);

        p->m_infoTipText = std::move(infoTipText);
        return p;
    }

    DECLARE_NOT_AGGREGATABLE(QueryInfo)

    BEGIN_COM_MAP(QueryInfo)
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

} // namespace msf
