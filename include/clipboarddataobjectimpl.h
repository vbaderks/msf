//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "cfhandler.h"
#include "enumformatetc.h"
#include "formatetc.h"
#include <atlctl.h> // for IDataObjectImpl
#include <memory>

namespace MSF
{

template<typename T>
class CClipboardDataObjectImpl : public ATL::IDataObjectImpl<T>
{
    class CExternalData;

    typedef std::vector<CCfHandler*>    CCfHandlers;
    typedef std::vector<CExternalData*> CExternalDatas;

public:

    CClipboardDataObjectImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::CClipboardDataObjectImpl (instance=%p)\n", this);
    }


    ~CClipboardDataObjectImpl() throw()
    {
        ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::~CClipboardDataObjectImpl (instance=%p)\n", this);

        for_each(m_cfhandlers, CDeleteObject());
        for_each(m_externaldatas, CDeleteObject());
    }


    void RegisterCfHandler(std::unique_ptr<CCfHandler> qcfhandler)
    {
        ATLASSERT(FindCfHandler(!qcfhandler->GetClipFormat()) && "Cannot register twice");
        m_cfhandlers.push_back(qcfhandler.get());
        qcfhandler.release();
    }


    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::EnumFormatEtc (dwDirection=%d)\n", dwDirection);

        try
        {
            if (dwDirection != DATADIR_GET && dwDirection != DATADIR_SET)
                return E_INVALIDARG;

            std::vector<FORMATETC> formatetcs;
            GetRegisteredFormats(formatetcs, dwDirection);
            GetExternalFormats(formatetcs);

            SHCreateStdEnumFmtEtc(formatetcs, ppenumFormatEtc);
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(QueryGetData)(FORMATETC* pformatetc)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::QueryGetData, cfformat=%d (%s)\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString());

        try
        {
            const CCfHandler* pcfhandler = FindCfHandler(pformatetc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                {
                    return pcfhandler->Validate(*pformatetc);
                }
            }
            else
            {
                const CExternalData* pexternaldata = FindExternalData(pformatetc->cfFormat);
                if (pexternaldata)
                {
                    return pexternaldata->Validate(*pformatetc);
                }
            }

            ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::QueryGetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(GetData)(FORMATETC *pformatetc, STGMEDIUM *pstgmedium)
    {
        ATLTRACE2(atlTraceCOM, 0, L"CClipboardDataObjectImpl::GetData cfformat=%d (%s)\n",
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
                CExternalData* pexternaldata = FindExternalData(pformatetc->cfFormat);
                if (pexternaldata)
                {
                    pexternaldata->Copy(*pstgmedium);
                    return S_OK;
                }
            }

            ATLTRACE2(atlTraceCOM, 0, _T("CClipboardDataObjectImpl::GetData (DV_E_FORMATETC)\n"));
            return DV_E_FORMATETC;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(SetData)(FORMATETC* pformatetc, STGMEDIUM* pstgmedium, BOOL fRelease)
    {
        ATLTRACE2(atlTraceCOM, 0, _T("CClipboardDataObjectImpl::SetData cfformat=%d (%s), tymed=%d, fRelease=%d\n"),
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

            if (!fRelease || pstgmedium->pUnkForRelease)
                return E_INVALIDARG; // external data can only be set, if we can take ownership.

            if (pstgmedium->tymed != TYMED_HGLOBAL &&
                pstgmedium->tymed != TYMED_ISTREAM &&
                pstgmedium->tymed != TYMED_ISTORAGE)
                return DV_E_TYMED; // only support for HGLOBAL and TYMED_ISTREAM.

            CExternalData* pexternaldata = FindExternalData(pformatetc->cfFormat);
            if (pexternaldata)
            {
                pexternaldata->Update(*pformatetc, *pstgmedium);
            }
            else
            {
                AddExternalData(*pformatetc, *pstgmedium);
            }

            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    // Required by ATL base class.
    LPDATAADVISEHOLDER m_spDataAdviseHolder;

private:

    class CExternalData
    {
    public:
        CExternalData(const FORMATETC& formatetc, const STGMEDIUM& stgmedium) throw() :
            _formatetc(formatetc),
            _stgmedium(stgmedium)
        {
        }


        CLIPFORMAT GetClipFormat() const throw()
        {
            return _formatetc.cfFormat;
        }


        const FORMATETC& GetFormatetc() const throw()
        {
            return _formatetc;
        }


        HRESULT Validate(const FORMATETC& formatetc) const throw()
        {
            if (formatetc.dwAspect != DVASPECT_CONTENT)
                return DV_E_DVASPECT;

            if (formatetc.tymed != _formatetc.tymed)
                return DV_E_TYMED;

            if (formatetc.lindex != -1)
                return DV_E_LINDEX;

            return S_OK;
        }


        void Update(const FORMATETC& formatetc, STGMEDIUM& stgmedium)
        {
            _formatetc = formatetc;
            _stgmedium = stgmedium;
        }


        void Copy(STGMEDIUM& stgmedium)
        {
            _stgmedium.CopyTo(stgmedium);
        }

    private:

        // Member variables.
        CFormatEtc _formatetc;
        CStgMedium _stgmedium;
    };


    CCfHandler* FindCfHandler(CLIPFORMAT clipformat) const throw()
    {
        for (CCfHandlers::const_iterator it = m_cfhandlers.begin(); it != m_cfhandlers.end(); ++it)
        {
            if ((*it)->GetClipFormat() == clipformat)
            {
                return *it;
            }
        }

        return nullptr;
    }


    CExternalData* FindExternalData(CLIPFORMAT clipformat) const throw()
    {
        for (CExternalDatas::const_iterator it = m_externaldatas.begin(); it != m_externaldatas.end(); ++it)
        {
            if ((*it)->GetClipFormat() == clipformat)
            {
                return *it;
            }
        }

        return nullptr;
    }


    void AddExternalData(const FORMATETC& formatetc, const STGMEDIUM& stgmedium)
    {
        ATLASSERT(!FindExternalData(formatetc.cfFormat) && "External format already set");

        std::unique_ptr<CExternalData> qexternaldata = make_unique<CExternalData>(formatetc, stgmedium);
        m_externaldatas.push_back(qexternaldata.get());
        qexternaldata.release();
    }


    void GetRegisteredFormats(std::vector<FORMATETC>& formatetcs, DWORD dwDirection) const
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


    void GetExternalFormats(std::vector<FORMATETC>& formatetcs) const
    {
        for (CExternalDatas::const_iterator it = m_externaldatas.begin(); it != m_externaldatas.end(); ++it)
        {
            formatetcs.push_back((*it)->GetFormatetc());
        }
    }


    // Member variables.
    CCfHandlers    m_cfhandlers;
    CExternalDatas m_externaldatas;
};

}
