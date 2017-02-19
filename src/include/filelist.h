//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "stgmedium.h"
#include <atlctl.h>


namespace MSF
{

class CFileList :
    public CComObjectRootEx<CComSingleThreadModel>,
    public ATL::IDataObjectImpl<CFileList>
{
public:
    DECLARE_NOT_AGGREGATABLE(CFileList)

    BEGIN_COM_MAP(CFileList)
        COM_INTERFACE_ENTRY(IDataObject)
    END_COM_MAP()


    static CComPtr<CFileList> CreateInstance()
    {
        CComObject<CFileList>* pinstance;
        RaiseExceptionIfFailed(
            CComObject<CFileList>::CreateInstance(&pinstance));

        CComPtr<CFileList> filelist(pinstance);

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
        MSF_COM_CATCH_HANDLER()
    }


    void Add(const wchar_t* szFilename)
    {
        m_filenames.push_back(szFilename);
    }


    LPDATAADVISEHOLDER m_spDataAdviseHolder;

protected:
    ~CFileList() = default;

private:
    static void ValidateFormatEtc(const FORMATETC* pformatetc)
    {
        RaiseExceptionIf(pformatetc->cfFormat != CF_HDROP, DV_E_FORMATETC);
        RaiseExceptionIf(pformatetc->dwAspect != DVASPECT_CONTENT, DV_E_DVASPECT);
        RaiseExceptionIf(pformatetc->lindex != -1, DV_E_LINDEX);
        RaiseExceptionIf(pformatetc->tymed != TYMED_HGLOBAL, DV_E_TYMED);
    }


    HGLOBAL CreateData() const
    {
        HGLOBAL hg = GlobalAllocThrow(GetDataSize());

        DROPFILES* pdropfiles = static_cast<DROPFILES*>(hg);

        pdropfiles->pFiles = sizeof(DROPFILES);
        pdropfiles->fWide = TRUE;

        // copy filenames into data block.
        wchar_t* pszBuf = reinterpret_cast<wchar_t*>(reinterpret_cast<BYTE*>(pdropfiles) +sizeof(DROPFILES));

        for (auto fileName : m_filenames)
        {
            ATLVERIFY(lstrcpy(pszBuf, fileName.GetString()));
            pszBuf += fileName.GetLength() + 1;
        }

        *pszBuf = L'\0';

        return hg;
    }


    size_t GetDataSize() const
    {
        UINT nchars = 0;

        for (auto fileName : m_filenames)
        {
            nchars += fileName.GetLength() + 1;
        }

        return sizeof(DROPFILES) + (sizeof(wchar_t) * (nchars + 1));
    }

    std::vector<CString> m_filenames;
};

} // end MSF namespace
