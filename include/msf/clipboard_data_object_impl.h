//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "cfhandler.h"
#include "enumformatetc.h"
#include "stgmedium.h"

#include <atlctl.h> // for IDataObjectImpl

#include <memory>
#include <algorithm>

namespace msf {

template<typename T>
class ClipboardDataObjectImpl : public ATL::IDataObjectImpl<T>
{
    class CExternalData;

    using CfHandlerVector = std::vector<std::unique_ptr<ClipboardFormatHandler>>;
    using ExternalDataVector = std::vector<std::unique_ptr<CExternalData>>;

public:
    ClipboardDataObjectImpl() noexcept
    {
        ATLTRACE(L"ClipboardDataObjectImpl::ClipboardDataObjectImpl (instance=%p)\n", this);
    }

    void RegisterCfHandler(std::unique_ptr<ClipboardFormatHandler> clipFormatHandler)
    {
        ATLASSERT(!FindClipFormatHandler(clipFormatHandler->GetClipFormat()) && "Cannot register a ClipBoard handler twice!");
        m_cfhandlers.push_back(clipFormatHandler);
    }

    HRESULT __stdcall EnumFormatEtc(DWORD direction, IEnumFORMATETC** /*enumFormatEtc*/) noexcept override
    {
        ATLTRACE(L"ClipboardDataObjectImpl::EnumFormatEtc (direction=%d)\n", direction);

        try
        {
            if (direction != DATADIR_GET && direction != DATADIR_SET)
                return E_INVALIDARG;

            std::vector<FORMATETC> formatEtcs;
            GetRegisteredFormats(formatEtcs, direction);
            GetExternalFormats(formatEtcs);

            uint16_t
            SHCreateStdEnumFmtEtc(static_cast<uint32_t>(formatEtcs.size()), formatEtcs.data());
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall QueryGetData(FORMATETC* formatEtc) noexcept override
    {
        ATLTRACE(L"ClipboardDataObjectImpl::QueryGetData, cfformat=%d (%s)\n",
                 formatEtc->cfFormat, GetClipboardFormatName(formatEtc->cfFormat).c_str());

        try
        {
            const ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(formatEtc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                {
                    return pcfhandler->Validate(*formatEtc);
                }
            }
            else
            {
                const CExternalData* externalData = FindExternalData(formatEtc->cfFormat);
                if (externalData)
                {
                    return externalData->Validate(*formatEtc);
                }
            }

            ATLTRACE(L"ClipboardDataObjectImpl::QueryGetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetData(FORMATETC *formatEtc, STGMEDIUM *storageMedium) noexcept override
    {
        ATLTRACE(L"ClipboardDataObjectImpl::GetData cfformat=%d (%s)\n",
                  formatEtc->cfFormat, GetClipboardFormatName(formatEtc->cfFormat).c_str());

        try
        {
            ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(formatEtc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                {
                    RaiseExceptionIfFailed(pcfhandler->Validate(*formatEtc));
                    pcfhandler->GetData(*formatEtc, *storageMedium);
                    return S_OK;
                }
            }
            else
            {
                CExternalData* pexternaldata = FindExternalData(formatEtc->cfFormat);
                if (pexternaldata)
                {
                    pexternaldata->Copy(*storageMedium);
                    return S_OK;
                }
            }

            ATLTRACE(L"ClipboardDataObjectImpl::GetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall SetData(FORMATETC* formatEtc, STGMEDIUM* storageMedium, BOOL release) noexcept override
    {
        ATLTRACE(L"ClipboardDataObjectImpl::SetData cfformat=%d (%s), tymed=%d, fRelease=%d\n",
                  formatEtc->cfFormat, GetClipboardFormatName(formatEtc->cfFormat).c_str(), formatEtc->tymed, release);

        try
        {
            if (formatEtc->ptd)
                return DV_E_DVTARGETDEVICE;

            if (formatEtc->tymed != storageMedium->tymed)
                return DV_E_TYMED;

            ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(formatEtc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanSetData())
                {
                    RaiseExceptionIfFailed(pcfhandler->Validate(*formatEtc));
                    pcfhandler->SetData(*formatEtc, *storageMedium, release);
                    return S_OK;
                }

                return E_FAIL;
            }

            if (!release || storageMedium->pUnkForRelease)
                return E_INVALIDARG; // external data can only be set, if we can take ownership.

            if (storageMedium->tymed != TYMED_HGLOBAL &&
                storageMedium->tymed != TYMED_ISTREAM &&
                storageMedium->tymed != TYMED_ISTORAGE)
                return DV_E_TYMED; // only support for HGLOBAL and TYMED_ISTREAM.

            CExternalData* pexternaldata = FindExternalData(formatEtc->cfFormat);
            if (pexternaldata)
            {
                pexternaldata->Update(*formatEtc, *storageMedium);
            }
            else
            {
                AddExternalData(*formatEtc, *storageMedium);
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
        ATLTRACE(L"ClipboardDataObjectImpl::~ClipboardDataObjectImpl (instance=%p)\n", this);
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

        [[nodiscard]] CLIPFORMAT GetClipFormat() const noexcept
        {
            return m_formatetc.cfFormat;
        }

        [[nodiscard]] const FORMATETC& GetFormatetc() const noexcept
        {
            return m_formatetc;
        }

        [[nodiscard]] HRESULT Validate(const FORMATETC& formatetc) const noexcept
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
        CFormatEtc m_formatetc;
        StorageMedium m_stgmedium;
    };

    [[nodiscard]] ClipboardFormatHandler* FindClipFormatHandler(CLIPFORMAT clipformat) const noexcept
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

    void GetRegisteredFormats(std::vector<FORMATETC>& formatetcs, DWORD direction) const
    {
        for (auto it = m_cfhandlers.begin(); it != m_cfhandlers.end(); ++it)
        {
            if (direction == DATADIR_GET)
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

    CfHandlerVector m_cfhandlers;
    CExternalDatas m_externaldatas;
};

}
