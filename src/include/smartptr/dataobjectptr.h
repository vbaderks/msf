//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "util.h"
#include "formatetc.h"
#include "enumformatetcptr.h"


namespace msf
{

class IDataObjectPtr : public ::IDataObjectPtr
{
public:
    IDataObjectPtr() noexcept : ::IDataObjectPtr()
    {
    }

    explicit IDataObjectPtr(const CLSID& clsid, IUnknown* pOuter = nullptr, DWORD dwClsContext = CLSCTX_ALL) :
        ::IDataObjectPtr(clsid, pOuter, dwClsContext)
    {
    }

    // Purpose: Constructs a smart-pointer from any IUnknown-based interface pointer.
    template<typename _InterfaceType> IDataObjectPtr(_InterfaceType* p) :
        ::IDataObjectPtr(p)
    {
    }

    // Purpose: Constructs a smart-pointer from any other smart pointer.
    template<typename _OtherIID> IDataObjectPtr(const _com_ptr_t<_OtherIID>& p) noexcept :
        ::IDataObjectPtr(p)
    {
    }

    IDataObjectPtr(IDataObject* pInterface) noexcept :
        ::IDataObjectPtr(pInterface)
    {
    }

    bool IsSupportedFormat(CLIPFORMAT clipformat, DWORD dwDirection)
    {
        IEnumFORMATETCPtr renumformatetc = EnumFormatEtc(dwDirection);

        FormatEtc formatetc;
        while (renumformatetc.Next(formatetc))
        {
            if (formatetc.cfFormat == clipformat)
                return true;

            formatetc.Dispose();
        }

        return false;
    }

    bool IsSupportedFormat(LPCTSTR lpszFormat, DWORD dwDirection)
    {
        return IsSupportedFormat(Win32::RegisterClipboardFormat(lpszFormat), dwDirection);
    }

    IEnumFORMATETCPtr EnumFormatEtc(DWORD dwDirection)
    {
        IEnumFORMATETCPtr renumformatetc;
        RaiseExceptionIfFailed(GetInterfacePtr()->EnumFormatEtc(dwDirection, &renumformatetc));
        return renumformatetc;
    }

    void GetData(const FORMATETC& formatetc, STGMEDIUM& stgmedium)
    {
#pragma warning(push)
#pragma warning(disable : 26493) // invalid warning about C-Style cast [visual studio 15.7.1]
        RaiseExceptionIfFailed(GetInterfacePtr()->GetData(&const_cast<FORMATETC&>(formatetc), &stgmedium));
#pragma warning(pop)
    }

    void SetData(const FORMATETC& formatetc, const STGMEDIUM& stgmedium, bool fRelease)
    {
#pragma warning(push)
#pragma warning(disable : 26493) // invalid warning about C-Style cast [visual studio 15.7.1]
        RaiseExceptionIfFailed(GetInterfacePtr()->SetData(&const_cast<FORMATETC&>(formatetc), &const_cast<STGMEDIUM&>(stgmedium), fRelease));
#pragma warning(pop)
    }
};

} // end msf namespace
