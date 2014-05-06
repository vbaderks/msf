//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfhandler.h"
#include "enumformatetc.h"
#include "formatetc.h"
#include "dataobjectptr.h"
#include "cfperformeddropeffecthandler.h"
#include <memory>

namespace MSF
{

template <typename T>
class CShellFolderDataObjectImpl : public IDataObject
{
public:
    typedef std::vector<CCfHandler*> CCfHandlers;
    typedef std::vector<CFormatEtc>  CFormatEtcs;


    CShellFolderDataObjectImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::CShellFolderDataObjectImpl (instance=%p)\n"), this);
    }


    ~CShellFolderDataObjectImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::~CShellFolderDataObjectImpl (instance=%p)\n"), this);

        for_each(m_cfhandlers, CDeleteObject());
    }


    void Init(LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST* ppidl,
        IPerformedDropEffectSink* pperformeddropeffectsink = nullptr)
    {
        m_pidldata = static_cast<IDataObject*>(CIDLData_CreateFromIDArray(pidlFolder, cidl, ppidl));
        RegisterCfHandler(make_unique<CCfPerformedDropEffectHandler>(pperformeddropeffectsink, this));
    }


    void RegisterCfHandler(std::unique_ptr<CCfHandler> qcfhandler)
    {
        ATLASSERT(FindCfHandler(!qcfhandler->GetClipFormat()) && "cannot register twice!");
        m_cfhandlers.push_back(qcfhandler.get());
        qcfhandler.release();
    }


    STDMETHOD(GetData)(_In_ FORMATETC* pformatetc, _Out_ STGMEDIUM* pstgmedium)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::GetData (cfformat=%d [%s])\n"), 
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString());

        try
        {
            CCfHandler* pcfhandler = FindCfHandler(pformatetc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                {
                    RaiseExceptionIfFailed(pcfhandler->Validate(*pformatetc));
                    pcfhandler->GetData(*pformatetc, *pstgmedium);
                    return S_OK;
                }
            }
            else
            {
                HRESULT hr = m_pidldata->GetData(pformatetc, pstgmedium);
                if (FAILED(hr))
                {
                    ATLTRACE2(atlTraceCOM, 0, _T("CClipboardDataObjectImpl::GetData (pidldata failes)\n"));
                }

                return hr;
            }

            ATLTRACE2(atlTraceCOM, 0, _T("CClipboardDataObjectImpl::GetData (DV_E_FORMATETC)\n"));
            return DV_E_FORMATETC;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(GetDataHere)(_In_ FORMATETC* pformatetc, _Inout_ STGMEDIUM* pmedium)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::GetDataHere (instance=%p)\n"), this);

        return m_pidldata->GetDataHere(pformatetc, pmedium);
    }


    STDMETHOD(QueryGetData)(__RPC__in_opt FORMATETC* pformatetc)
    {
        // The docs define pformatetc as [in]. The SDK defines pformatetc as in_opt.
        if (!pformatetc)
        {
            ATLTRACE2(atlTraceCOM, 0, L"CShellFolderDataObjectImpl::QueryGetData (!pformatetc)\n");
            return DV_E_FORMATETC;
        }

        ATLTRACE2(atlTraceCOM, 0, L"CShellFolderDataObjectImpl::QueryGetData, cfformat=%d (%s)\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString());

        try
        {
            const CCfHandler* pcfhandler = FindCfHandler(pformatetc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                    return pcfhandler->Validate(*pformatetc);

                ATLTRACE2(atlTraceCOM, 0, _T("CClipboardDataObjectImpl::QueryGetData (DV_E_FORMATETC)\n"));
                return DV_E_FORMATETC;
            }
            else
            {
                return m_pidldata->QueryGetData(pformatetc);
            }
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(GetCanonicalFormatEtc)(__RPC__in_opt FORMATETC* pformatetc, __RPC__out FORMATETC* pformatetcOut)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::GetCanonicalFormatEtc (instance=%p)\n"), this);

        return m_pidldata->GetCanonicalFormatEtc(pformatetc, pformatetcOut);
    }


    STDMETHOD(SetData)(_In_ FORMATETC* pformatetc, _In_ STGMEDIUM* pstgmedium, BOOL fRelease)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::SetData cfformat=%d (%s), tymed=%d, fRelease=%d\n"),
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString(), pformatetc->tymed, fRelease);

        try
        {
            if (pformatetc->ptd)
                return DV_E_DVTARGETDEVICE;

            if (pformatetc->tymed != pstgmedium->tymed)
                return DV_E_TYMED;

            CCfHandler* pcfhandler = FindCfHandler(pformatetc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanSetData())
                {
                    RaiseExceptionIfFailed(pcfhandler->Validate(*pformatetc));
                    pcfhandler->SetData(*pformatetc, *pstgmedium, fRelease > 0);
                    return S_OK;
                }

                return E_FAIL;
            }

            return m_pidldata->SetData(pformatetc, pstgmedium, fRelease);
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, _In_ IEnumFORMATETC** ppenumFormatEtc)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::EnumFormatEtc (dwDirection=%d)\n"), dwDirection);

        try
        {
            if (dwDirection != DATADIR_GET && dwDirection != DATADIR_SET)
                return E_INVALIDARG;

            CFormatEtcs formatetcs;
            GetRegisteredFormats(dwDirection, formatetcs);
            GetPidlDataFormats(dwDirection, formatetcs);

            *ppenumFormatEtc = SHCreateStdEnumFmtEtc(static_cast<UINT>(formatetcs.size()), &(formatetcs[0])).Detach();

            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(DAdvise)(__RPC__in FORMATETC* pformatetc, DWORD advf, __RPC__in_opt IAdviseSink* pAdvSink, __RPC__out DWORD* pdwConnection)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::DAdvise (instance=%p)\n"), this);

        return m_pidldata->DAdvise(pformatetc, advf, pAdvSink, pdwConnection);
    }


    STDMETHOD(DUnadvise)(DWORD dwConnection)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::DUnadvise (instance=%p)\n"), this);

        return m_pidldata->DUnadvise(dwConnection);
    }


    STDMETHOD(EnumDAdvise)(__RPC__deref_out_opt IEnumSTATDATA** ppenumAdvise)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CShellFolderDataObjectImpl::EnumDAdvise (instance=%p)\n"), this);

        return m_pidldata->EnumDAdvise(ppenumAdvise);
    }


private:

    CCfHandler* FindCfHandler(CLIPFORMAT clipformat) const throw()
    {
        for (CCfHandlers::const_iterator it = m_cfhandlers.begin(); it != m_cfhandlers.end(); ++it)
        {
            if ((*it)->GetClipFormat() == clipformat)
            {
                return *it;
            }
        }

        return NULL;
    }


    void GetRegisteredFormats(DWORD dwDirection, CFormatEtcs& formatetcs) const
    {
        for (CCfHandlers::const_iterator it = m_cfhandlers.begin(); it != m_cfhandlers.end(); ++it)
        {
            if (dwDirection == DATADIR_GET)
            {
                if ((*it)->CanGetData())
                {
                    formatetcs.push_back(CFormatEtc((*it)->GetClipFormat()));
                }
            }
            else
            {
                if ((*it)->CanSetData())
                {
                    formatetcs.push_back(CFormatEtc((*it)->GetClipFormat()));
                }
            }
        }
    }


    void GetPidlDataFormats(DWORD dwDirection, CFormatEtcs& formatetcs)
    {
        IEnumFORMATETCPtr renumformatetc = m_pidldata.EnumFormatEtc(dwDirection);

        CFormatEtc formatetc;
        while (renumformatetc.Next(formatetc))
        {
            formatetcs.push_back(formatetc);
        }
    }


    // Member variables.
    IDataObjectPtr m_pidldata;
    CCfHandlers    m_cfhandlers;
};

} // end MSF namespace

