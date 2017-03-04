//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/enumidlistimpl.h"
#include "vvvfile.h"
#include <memory>


class ATL_NO_VTABLE EnumIDList :
    public CComObjectRootEx<CComSingleThreadModel>,
    public MSF::IEnumIDListImpl<EnumIDList>
{
public:
    DECLARE_NOT_AGGREGATABLE(EnumIDList)

    BEGIN_COM_MAP(EnumIDList)
        COM_INTERFACE_ENTRY(IEnumIDList)
    END_COM_MAP()

    static CComPtr<IEnumIDList> CreateInstance(const std::wstring& strFilename, const std::wstring& strFolder, DWORD grfFlags)
    {
        CComObject<EnumIDList>* pinstance;
        HRESULT hr = CComObject<EnumIDList>::CreateInstance(&pinstance);
        if (FAILED(hr))
            MSF::RaiseException(hr);

        CComPtr<IEnumIDList> renumidlist(pinstance);
        pinstance->Initialize(strFilename, strFolder, grfFlags);
        return renumidlist;
    }

    LPITEMIDLIST GetNextItem()
    {
        return m_file->GetNextItem(m_grfFlags, m_nItemIterator);
    }

protected:

    EnumIDList() : m_grfFlags(0), m_nItemIterator(0)
    {
    }

private:

    void Initialize(const std::wstring& strFilename, const std::wstring& strFolder, DWORD grfFlags)
    {
        m_file = std::make_unique<VVVFile>(strFilename, strFolder);
        m_grfFlags = grfFlags;
    }

    // Member variables
    DWORD m_grfFlags;
    std::unique_ptr<VVVFile> m_file;
    unsigned int m_nItemIterator;
};
