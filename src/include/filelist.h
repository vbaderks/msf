//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "stgmedium.h"
#include <atlctl.h>


namespace msf
{

class FileList :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::IDataObjectImpl<FileList>
{
public:
    DECLARE_NOT_AGGREGATABLE(FileList)

    BEGIN_COM_MAP(FileList)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()

    static ATL::CComPtr<FileList> CreateInstance()
    {
        ATL::CComObject<FileList>* pinstance;
        RaiseExceptionIfFailed(ATL::CComObject<FileList>::CreateInstance(&pinstance));

        ATL::CComPtr<FileList> filelist(pinstance);

        return filelist;
    }

    virtual HRESULT IDataObject_GetData(FORMATETC* pformatetc, STGMEDIUM* pmedium)
    {
        try
        {
            ValidateFormatEtc(pformatetc);

            pmedium->tymed = TYMED_HGLOBAL;
            pmedium->pUnkForRelease = nullptr;
            pmedium->hGlobal = CreateData();

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    void Add(const wchar_t* filename)
    {
        m_filenames.emplace_back(filename);
    }

    LPDATAADVISEHOLDER m_spDataAdviseHolder;

protected:
    ~FileList() = default;

private:
    static void ValidateFormatEtc(const FORMATETC* formatEtc)
    {
        RaiseExceptionIf(formatEtc->cfFormat != CF_HDROP, DV_E_FORMATETC);
        RaiseExceptionIf(formatEtc->dwAspect != DVASPECT_CONTENT, DV_E_DVASPECT);
        RaiseExceptionIf(formatEtc->lindex != -1, DV_E_LINDEX);
        RaiseExceptionIf(formatEtc->tymed != TYMED_HGLOBAL, DV_E_TYMED);
    }

    HGLOBAL CreateData() const
    {
        const HGLOBAL hg = GlobalAllocThrow(GetDataSize());

        DROPFILES* pdropfiles = static_cast<DROPFILES*>(hg);

        pdropfiles->pFiles = sizeof(DROPFILES);
        pdropfiles->fWide = TRUE;

        // copy filenames into data block.
        wchar_t* pszBuf = reinterpret_cast<wchar_t*>(reinterpret_cast<BYTE*>(pdropfiles) +sizeof(DROPFILES));

        for (auto fileName : m_filenames)
        {
            ATLVERIFY(lstrcpy(pszBuf, fileName.c_str()));
            pszBuf += fileName.GetLength() + 1;
        }

        *pszBuf = L'\0';

        return hg;
    }

    size_t GetDataSize() const
    {
        uint32_t nchars = 0;

        for (const auto& fileName : m_filenames)
        {
            nchars += fileName.GetLength() + 1;
        }

        return sizeof(DROPFILES) + (sizeof(wchar_t) * (nchars + 1));
    }

    std::vector<std::wstring> m_filenames;
};

} // end msf namespace
