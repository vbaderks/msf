//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "vvvfile.h"
#include <msf.h>
#include <memory>


class __declspec(novtable) EnumIDList :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public msf::IEnumIDListImpl<EnumIDList>
{
public:
    DECLARE_NOT_AGGREGATABLE(EnumIDList)

    BEGIN_COM_MAP(EnumIDList)
        COM_INTERFACE_ENTRY(IEnumIDList)
    END_COM_MAP()

    static ATL::CComPtr<IEnumIDList> CreateInstance(const std::wstring& strFilename, const std::wstring& strFolder, DWORD grfFlags)
    {
        ATL::CComObject<EnumIDList>* pinstance;
        const HRESULT hr = ATL::CComObject<EnumIDList>::CreateInstance(&pinstance);
        if (FAILED(hr))
            msf::RaiseException(hr);

        ATL::CComPtr<IEnumIDList> renumidlist(pinstance);
        pinstance->Initialize(strFilename, strFolder, grfFlags);
        return renumidlist;
    }

    LPITEMIDLIST GetNextItem()
    {
        return m_file->GetNextItem(m_grfFlags, m_nItemIterator);
    }

protected:
    EnumIDList() noexcept : m_grfFlags(0), m_nItemIterator(0)
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
