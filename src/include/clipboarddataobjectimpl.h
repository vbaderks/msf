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
#include "stgmedium.h"
#include <atlctl.h> // for IDataObjectImpl
#include <memory>
#include <algorithm>

namespace MSF {

template<typename T>
class ClipboardDataObjectImpl : public ATL::IDataObjectImpl<T>
{
    class CExternalData;

    typedef std::vector<std::unique_ptr<ClipboardFormatHandler>> CCfHandlers;
    typedef std::vector<std::unique_ptr<CExternalData>> CExternalDatas;

public:

    ClipboardDataObjectImpl() noexcept
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::ClipboardDataObjectImpl (instance=%p)\n", this);
    }

    void RegisterCfHandler(std::unique_ptr<ClipboardFormatHandler> clipFormatHandler)
    {
        ATLASSERT(!FindClipFormatHandler(clipFormatHandler->GetClipFormat()) && "Cannot register a ClipBoard handler twice!");
        m_cfhandlers.push_back(clipFormatHandler);
    }

    STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC** /*ppenumFormatEtc*/) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::EnumFormatEtc (dwDirection=%d)\n", dwDirection);

        try
        {
            if (dwDirection != DATADIR_GET && dwDirection != DATADIR_SET)
                return E_INVALIDARG;

            std::vector<FORMATETC> formatetcs;
            GetRegisteredFormats(formatetcs, dwDirection);
            GetExternalFormats(formatetcs);

            SHCreateStdEnumFmtEtc(static_cast<UINT>(formatetcs.size()), formatetcs.data());
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(QueryGetData)(FORMATETC* pformatetc) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::QueryGetData, cfformat=%d (%s)\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString());

        try
        {
            const ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
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

            ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::QueryGetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(GetData)(FORMATETC *pformatetc, STGMEDIUM *pstgmedium) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::GetData cfformat=%d (%s)\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString());

        try
        {
            ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
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

            ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::GetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    STDMETHOD(SetData)(FORMATETC* pformatetc, STGMEDIUM* pstgmedium, BOOL fRelease) override
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::SetData cfformat=%d (%s), tymed=%d, fRelease=%d\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).GetString(), pformatetc->tymed, fRelease);

        try
        {
            if (pformatetc->ptd)
                return DV_E_DVTARGETDEVICE;

            if (pformatetc->tymed != pstgmedium->tymed)
                return DV_E_TYMED;

            ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
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
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    // Required by ATL base class.
    LPDATAADVISEHOLDER m_spDataAdviseHolder;

protected:

    ~ClipboardDataObjectImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"ClipboardDataObjectImpl::~ClipboardDataObjectImpl (instance=%p)\n", this);
    }

private:

    class CExternalData
    {
    public:
        CExternalData(const FORMATETC& formatetc, const STGMEDIUM& stgmedium) noexcept :
            m_formatetc(formatetc),
            m_stgmedium(stgmedium)
        {
        }

        CLIPFORMAT GetClipFormat() const noexcept
        {
            return m_formatetc.cfFormat;
        }

        const FORMATETC& GetFormatetc() const noexcept
        {
            return m_formatetc;
        }

        HRESULT Validate(const FORMATETC& formatetc) const noexcept
        {
            if (formatetc.dwAspect != DVASPECT_CONTENT)
                return DV_E_DVASPECT;

            if (formatetc.tymed != m_formatetc.tymed)
                return DV_E_TYMED;

            if (formatetc.lindex != -1)
                return DV_E_LINDEX;

            return S_OK;
        }

        void Update(const FORMATETC& formatetc, STGMEDIUM& stgmedium)
        {
            m_formatetc = formatetc;
            m_stgmedium = stgmedium;
        }

        void Copy(STGMEDIUM& stgmedium) const
        {
            m_stgmedium.CopyTo(stgmedium);
        }

    private:

        // Member variables.
        CFormatEtc m_formatetc;
        CStgMedium m_stgmedium;
    };

    ClipboardFormatHandler* FindClipFormatHandler(CLIPFORMAT clipformat) const noexcept
    {
        auto handler = std::find_if(m_cfhandlers.begin(), m_cfhandlers.end(),
            [=](ClipboardFormatHandler* cfHandler)
        { 
            return cfHandler->GetClipFormat() == clipformat;
        });
        return handler == m_cfhandlers.end() ? nullptr : handler;
    }

    CExternalData* FindExternalData(CLIPFORMAT clipformat) const noexcept
    {
        for (typename CExternalDatas::const_iterator it = m_externaldatas.begin(); it != m_externaldatas.end(); ++it)
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
        m_externaldatas.push_back(std::make_unique<CExternalData>(formatetc, stgmedium));
    }

    void GetRegisteredFormats(std::vector<FORMATETC>& formatetcs, DWORD dwDirection) const
    {
        for (auto it = m_cfhandlers.begin(); it != m_cfhandlers.end(); ++it)
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
        for (auto externalData : m_externaldatas)
        {
            formatetcs.push_back(externalData->GetFormatetc());
        }
    }

    // Member variables.
    CCfHandlers    m_cfhandlers;
    CExternalDatas m_externaldatas;
};

}
