//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "util.h"

namespace MSF
{

/// <summary>Smart extended class for Windows SDK FORMATETC struct.</summary>
/// <remarks>
/// The CFormatEtc class extends FORMATETC with typicals constructors and a 
/// destructor that will clean up any dynamic allocated memory.
/// </remarks>
class CFormatEtc : public FORMATETC
{
public:
    CFormatEtc(CLIPFORMAT cfformat, DWORD dwtymed = TYMED_HGLOBAL, DVTARGETDEVICE* pdvtd = nullptr, DWORD dwaspect = DVASPECT_CONTENT, LONG index = -1) throw()
    {
        CommonConstruct(cfformat, dwtymed, pdvtd, dwaspect, index);
    }

    CFormatEtc(LPCTSTR lpszFormat, DWORD dwtymed = TYMED_HGLOBAL, DVTARGETDEVICE* pdvtd = nullptr, DWORD dwaspect = DVASPECT_CONTENT, LONG index = -1) throw()
    {
        CommonConstruct(RegisterCf(lpszFormat), dwtymed, pdvtd, dwaspect, index);
    }

    CFormatEtc()
    {
        ptd = nullptr;
    }

    CFormatEtc(const FORMATETC& formatetc)
    {
        Copy(*this, formatetc);
    }

    ~CFormatEtc()
    {
        Dispose();
    }

    CFormatEtc& operator=(const FORMATETC& formatetc)
    {
        if (this == &formatetc)
            return *this;

        Dispose();
        Copy(*this, formatetc);
        return *this;
    }

    void Dispose() throw()
    {
        if (ptd)
        {
            CoTaskMemFree(ptd);
            ptd = nullptr;
        }
    }

private:

    void CommonConstruct(CLIPFORMAT cfformat, DWORD dwtymed, DVTARGETDEVICE* pdvtd, DWORD dwaspect, LONG index) throw()
    {
        cfFormat = cfformat;
        tymed    = dwtymed;
        ptd      = pdvtd;
        dwAspect = dwaspect;
        lindex   = index;
    }

    static void Copy(FORMATETC& dest, const FORMATETC& src)
    {
        DVTARGETDEVICE* ptd = CopyTargetDevice(src);
        dest = src;
        dest.ptd = ptd;
    }

    static DVTARGETDEVICE* CopyTargetDevice(const FORMATETC& src)
    {
        if (!src.ptd)
            return nullptr;

        DVTARGETDEVICE* ptd = static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(src.ptd->tdSize));
        if (!ptd)
            throw std::bad_alloc();
        memcpy(ptd, src.ptd, src.ptd->tdSize);
        return ptd;
    }
};

} // namespace MSF
