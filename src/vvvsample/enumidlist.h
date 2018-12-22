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

    static ATL::CComPtr<IEnumIDList> CreateInstance(const std::wstring& filename, const std::wstring& folder, DWORD grfFlags)
    {
        ATL::CComObject<EnumIDList>* instance;
        const HRESULT hr = ATL::CComObject<EnumIDList>::CreateInstance(&instance);
        if (FAILED(hr))
            msf::RaiseException(hr);

        ATL::CComPtr<IEnumIDList> enumIdList(instance);
        instance->Initialize(filename, folder, grfFlags);
        return enumIdList;
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
    void Initialize(const std::wstring& filename, const std::wstring& folder, DWORD grfFlags)
    {
        m_file = std::make_unique<VVVFile>(filename, folder);
        m_grfFlags = grfFlags;
    }

    // Member variables
    DWORD m_grfFlags;
    std::unique_ptr<VVVFile> m_file;
    unsigned int m_nItemIterator;
};
