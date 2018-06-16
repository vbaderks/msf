//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <memory>

namespace msf
{

inline ATL::CComPtr<IEnumFORMATETC> SHCreateStdEnumFmtEtc(UINT cfmt, const FORMATETC* pformatetc)
{
    ATL::CComPtr<IEnumFORMATETC> renumformatetc;
    RaiseExceptionIfFailed(::SHCreateStdEnumFmtEtc(cfmt, pformatetc, &renumformatetc));
    return renumformatetc;
}


// Purpose: copy policy class: required for the ATL::CComEnumOnSTL template.
class FormatEtcToFormatEtc
{
public:
    static void init(FORMATETC*) noexcept
    {
        // No init needed.
    }

    static HRESULT copy(FORMATETC* pTo, const FORMATETC* pFrom) noexcept
    {
        *pTo = *pFrom;
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

    static ATL::CComPtr<EnumFORMATETC> CreateInstance(std::unique_ptr<FormatEtcs> qformatetcs)
    {
        ATL::CComObject<EnumFORMATETC>* pEnum;
        RaiseExceptionIfFailed(ATL::CComObject<EnumFORMATETC>::CreateInstance(&pEnum));

        ATL::CComPtr<EnumFORMATETC> renum(pEnum);
        renum->Initialize(move(qformatetcs));

        return renum;
    }

protected:
    EnumFORMATETC() noexcept
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"CEnumFORMATETC::CEnumFORMATETC (instance=%p)\n", this);
    }

    ~EnumFORMATETC()
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"CEnumFORMATETC::~CEnumFORMATETC (instance=%p)\n", this);
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
inline HRESULT CreateStdEnumFmtEtc(std::unique_ptr<std::vector<FORMATETC> > qformatetcs, IEnumFORMATETC** ppenumFormatEtc)
{
    try
    {
        ATL::CComPtr<EnumFORMATETC> renum = EnumFORMATETC::CreateInstance(std::move(qformatetcs));
        *ppenumFormatEtc = renum.Detach();

        return S_OK;
    }
    catch (...)
    {
        return ExceptionToHResult();
    }
}

} // end of msf namespace
