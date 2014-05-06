//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/enumidlistimpl.h"
#include "vvvfile.h"
#include <memory>


class ATL_NO_VTABLE CEnumIDList :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IEnumIDListImpl<CEnumIDList>
{
public:
    DECLARE_NOT_AGGREGATABLE(CEnumIDList)

    BEGIN_COM_MAP(CEnumIDList)
        COM_INTERFACE_ENTRY(IEnumIDList)
    END_COM_MAP()

    static CComPtr<IEnumIDList> CreateInstance(const CString& strFilename, const CString& strFolder, DWORD grfFlags)
    {
        CComObject<CEnumIDList>* pinstance;
        HRESULT hr = CComObject<CEnumIDList>::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        CComPtr<IEnumIDList> renumidlist(pinstance);
        pinstance->Initialize(strFilename, strFolder, grfFlags);
        return renumidlist;
    }


    LPITEMIDLIST GetNextItem()
    {
        return m_qfile->GetNextItem(m_grfFlags, m_nItemIterator);
    }

private:

    void Initialize(const CString& strFilename, const CString& strFolder, DWORD grfFlags)
    {
        m_qfile = make_unique<CVVVFile>(strFilename, strFolder);
        m_grfFlags = grfFlags;
        m_nItemIterator = 0;
    }

    // Member variables
    DWORD m_grfFlags;
    unique_ptr<CVVVFile> m_qfile;
    unsigned int m_nItemIterator;
};
