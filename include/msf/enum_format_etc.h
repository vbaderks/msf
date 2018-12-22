//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"

#include <memory>

namespace msf
{

inline ATL::CComPtr<IEnumFORMATETC> SHCreateStdEnumFmtEtc(uint32_t cfmt, const FORMATETC* formatetc)
{
    ATL::CComPtr<IEnumFORMATETC> enumformatetc;
    RaiseExceptionIfFailed(::SHCreateStdEnumFmtEtc(cfmt, formatetc, &enumformatetc));
    return enumformatetc;
}


// Purpose: copy policy class: required for the ATL::CComEnumOnSTL template.
class FormatEtcToFormatEtc
{
public:
    static void init(FORMATETC*) noexcept
    {
        // No init needed.
    }

    static HRESULT copy(FORMATETC* to, const FORMATETC* from) noexcept
    {
        *to = *from;
        return S_OK;
    }

    static void destroy(FORMATETC*) noexcept
    {
    }
};


class EnumFORMATETC :
    public ATL::CComEnumOnSTL<IEnumFORMATETC,
                              &IID_IEnumFORMATETC,    // name and IID of enumerator interface
                              FORMATETC,              // type of object to return
                              FormatEtcToFormatEtc,  // copy policy class
                              std::vector<FORMATETC>> // type of collection holding the data
{
public:
    using FormatEtcs = std::vector<FORMATETC>;

    EnumFORMATETC(const EnumFORMATETC&) = delete;
    EnumFORMATETC(EnumFORMATETC&&) = delete;
    EnumFORMATETC& operator=(const EnumFORMATETC&) = delete;
    EnumFORMATETC& operator=(EnumFORMATETC&&) = delete;

    static ATL::CComPtr<EnumFORMATETC> CreateInstance(std::unique_ptr<FormatEtcs> formatEtcs)
    {
        ATL::CComObject<EnumFORMATETC>* enumFormatEtc;
        RaiseExceptionIfFailed(ATL::CComObject<EnumFORMATETC>::CreateInstance(&enumFormatEtc));

        ATL::CComPtr<EnumFORMATETC> enumPtr(enumFormatEtc);
        enumPtr->Initialize(std::move(formatEtcs));

        return enumPtr;
    }

protected:
    EnumFORMATETC() noexcept
    {
        ATLTRACE(L"CEnumFORMATETC::CEnumFORMATETC (instance=%p)\n", this);
    }

    ~EnumFORMATETC()
    {
        ATLTRACE(L"CEnumFORMATETC::~CEnumFORMATETC (instance=%p)\n", this);
    }

private:
    void Initialize(std::unique_ptr<FormatEtcs> formatEtcs) noexcept
    {
        m_formatEtcs = std::move(formatEtcs);

        ATLVERIFY(SUCCEEDED(__super::Init(this, *m_formatEtcs)));
    }

    // Member variables.
    std::unique_ptr<FormatEtcs> m_formatEtcs;
};


// Purpose: alternative for the SHCreateStdEnumFmtEtc function.
//          Created before I discovered SHCreateStdEnumFmtEtc.
//          Source can act as sample, or as easy debug alternative.
inline HRESULT CreateStdEnumFmtEtc(std::unique_ptr<std::vector<FORMATETC>> formatEtcs, IEnumFORMATETC** enumFormatEtc)
{
    try
    {
        ATL::CComPtr<EnumFORMATETC> enumPtr = EnumFORMATETC::CreateInstance(std::move(formatEtcs));
        *enumFormatEtc = enumPtr.Detach();

        return S_OK;
    }
    catch (...)
    {
        return ExceptionToHResult();
    }
}

} // end of msf namespace
