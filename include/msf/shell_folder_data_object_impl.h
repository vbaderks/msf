//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cf_handler.h"
#include "enum_format_etc.h"
#include "format_etc.h"
#include "cf_performed_drop_effect_handler.h"
#include "idldatacreatefromidarray.h"
#include "smartptr/dataobjectptr.h"

#include <memory>
#include <algorithm>

namespace msf
{

template <typename T>
class ShellFolderDataObjectImpl : public IDataObject
{
public:
    ShellFolderDataObjectImpl(const ShellFolderDataObjectImpl&) = delete;
    ShellFolderDataObjectImpl(ShellFolderDataObjectImpl&&) = delete;
    ShellFolderDataObjectImpl& operator=(const ShellFolderDataObjectImpl&) = delete;
    ShellFolderDataObjectImpl& operator=(ShellFolderDataObjectImpl&&) = delete;

    HRESULT __stdcall GetData(_In_ FORMATETC* pformatetc, _Out_ STGMEDIUM* pstgmedium) noexcept override
    {
        ATLTRACE("ShellFolderDataObjectImpl::GetData (cfformat=%d [%s])\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).c_str());

        try
        {
            const ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
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
                const auto hr = m_pidldata->GetData(pformatetc, pstgmedium);
                if (FAILED(hr))
                {
                    ATLTRACE(L"ClipboardDataObjectImpl::GetData (pidldata failed)\n");
                }

                return hr;
            }

            ATLTRACE(L"ClipboardDataObjectImpl::GetData (DV_E_FORMATETC)\n");
            return DV_E_FORMATETC;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetDataHere(_In_ FORMATETC* pformatetc, _Inout_ STGMEDIUM* pmedium) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::GetDataHere (instance=%p)\n", this);

        return m_pidldata->GetDataHere(pformatetc, pmedium);
    }

    HRESULT __stdcall QueryGetData(__RPC__in_opt FORMATETC* pformatetc) noexcept override
    {
        // The docs define pformatetc as [in]. The SDK defines pformatetc as in_opt.
        if (!pformatetc)
        {
            ATLTRACE(L"ShellFolderDataObjectImpl::QueryGetData (!pformatetc)\n");
            return DV_E_FORMATETC;
        }

        ATLTRACE(L"ShellFolderDataObjectImpl::QueryGetData, cfformat=%d (%s)\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).c_str());

        try
        {
            const ClipboardFormatHandler* pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
            if (pcfhandler)
            {
                if (pcfhandler->CanGetData())
                    return pcfhandler->Validate(*pformatetc);

                ATLTRACE(L"ClipboardDataObjectImpl::QueryGetData (DV_E_FORMATETC)\n");
                return DV_E_FORMATETC;
            }

            return m_pidldata->QueryGetData(pformatetc);
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall GetCanonicalFormatEtc(__RPC__in_opt FORMATETC* pformatetc, __RPC__out FORMATETC* pformatetcOut) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::GetCanonicalFormatEtc (instance=%p)\n", this);

        return m_pidldata->GetCanonicalFormatEtc(pformatetc, pformatetcOut);
    }

    HRESULT __stdcall SetData(_In_ FORMATETC* pformatetc, _In_ STGMEDIUM* pstgmedium, BOOL fRelease) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::SetData cfformat=%d (%s), tymed=%d, fRelease=%d\n",
            pformatetc->cfFormat, GetClipboardFormatName(pformatetc->cfFormat).c_str(), pformatetc->tymed, fRelease);

        try
        {
            if (pformatetc->ptd)
                return DV_E_DVTARGETDEVICE;

            if (pformatetc->tymed != pstgmedium->tymed)
                return DV_E_TYMED;

            auto pcfhandler = FindClipFormatHandler(pformatetc->cfFormat);
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
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall EnumFormatEtc(DWORD dwDirection, _In_ IEnumFORMATETC** ppenumFormatEtc) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::EnumFormatEtc (dwDirection=%d)\n", dwDirection);

        try
        {
            if (dwDirection != DATADIR_GET && dwDirection != DATADIR_SET)
                return E_INVALIDARG;

            std::vector<FormatEtc> formatEtcs;
            GetRegisteredFormats(dwDirection, formatEtcs);
            GetPidlDataFormats(dwDirection, formatEtcs);

            *ppenumFormatEtc = SHCreateStdEnumFmtEtc(static_cast<uint32_t>(formatEtcs.size()), formatEtcs.data()).Detach();

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall DAdvise(__RPC__in FORMATETC* pformatetc, DWORD advf, __RPC__in_opt IAdviseSink* pAdvSink, __RPC__out DWORD* pdwConnection) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::DAdvise (instance=%p)\n", this);

        return m_pidldata->DAdvise(pformatetc, advf, pAdvSink, pdwConnection);
    }

    HRESULT __stdcall DUnadvise(DWORD dwConnection) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::DUnadvise (instance=%p)\n", this);

        return m_pidldata->DUnadvise(dwConnection);
    }

    HRESULT __stdcall EnumDAdvise(__RPC__deref_out_opt IEnumSTATDATA** ppenumAdvise) noexcept override
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::EnumDAdvise (instance=%p)\n", this);

        return m_pidldata->EnumDAdvise(ppenumAdvise);
    }

protected:
    ShellFolderDataObjectImpl() noexcept
    {
        ATLTRACE(L"ShellFolderDataObjectImpl::ShellFolderDataObjectImpl (instance=%p)\n", this);
    }

    ~ShellFolderDataObjectImpl()
    {
        ATLTRACE("ShellFolderDataObjectImpl::~ShellFolderDataObjectImpl (instance=%p)\n", this);
    }

    void Init(PCIDLIST_ABSOLUTE pidlFolder, uint32_t cidl, PCUITEMID_CHILD_ARRAY ppidl,
        IPerformedDropEffectSink* pperformeddropeffectsink = nullptr)
    {
        m_pidldata = static_cast<IDataObject*>(CIDLData_CreateFromIDArray(pidlFolder, cidl, reinterpret_cast<PCUIDLIST_RELATIVE_ARRAY>(ppidl)));
        RegisterClipboardFormatHandler(std::make_unique<ClipboardPerformedDropEffectHandler>(pperformeddropeffectsink, this));
    }

    void RegisterClipboardFormatHandler(std::unique_ptr<ClipboardFormatHandler> qcfhandler)
    {
        ATLASSERT(!FindClipFormatHandler(qcfhandler->GetClipFormat()) && "Cannot register a ClipBoard handler twice!");
        m_cfhandlers.push_back(std::move(qcfhandler));
    }

private:
    [[nodiscard]] ClipboardFormatHandler* FindClipFormatHandler(CLIPFORMAT clipFormat) const noexcept
    {
        auto handler = std::find_if(m_cfhandlers.begin(), m_cfhandlers.end(),
            [=](const std::unique_ptr<ClipboardFormatHandler>& clipFormatHandler) noexcept
        {
            return clipFormatHandler->GetClipFormat() == clipFormat;
        });
        return handler == m_cfhandlers.end() ? nullptr : (*handler).get();
    }

    void GetRegisteredFormats(DWORD direction, std::vector<FormatEtc>& formatEtcs) const
    {
        for (const auto& handler : m_cfhandlers)
        {
            if (direction == DATADIR_GET)
            {
                if (handler->CanGetData())
                {
                    formatEtcs.push_back(FormatEtc(handler->GetClipFormat()));
                }
            }
            else
            {
                if (handler->CanSetData())
                {
                    formatEtcs.push_back(FormatEtc(handler->GetClipFormat()));
                }
            }
        }
    }

    void GetPidlDataFormats(DWORD dwDirection, std::vector<FormatEtc>& formatEtcs)
    {
        IEnumFORMATETCPtr enumFormatEtc = m_pidldata.EnumFormatEtc(dwDirection);

        FormatEtc formatEtc;
        while (enumFormatEtc.Next(formatEtc))
        {
            formatEtcs.push_back(formatEtc);
        }
    }

    // Member variables.
    IDataObjectPtr m_pidldata;
    std::vector<std::unique_ptr<ClipboardFormatHandler>> m_cfhandlers;
};

} // end msf namespace

