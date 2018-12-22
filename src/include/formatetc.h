//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "util.h"

namespace msf
{

/// <summary>Smart extended class for Windows SDK FORMATETC struct.</summary>
/// <remarks>
/// The CFormatEtc class extends FORMATETC with typical constructors and a
/// destructor that will clean up any dynamic allocated memory.
/// </remarks>
class FormatEtc : public FORMATETC
{
public:
    FormatEtc() noexcept : FORMATETC()
    {
        ptd = nullptr;
    }

    explicit FormatEtc(const FORMATETC& formatetc) : FORMATETC()
    {
        Copy(*this, formatetc);
    }

    explicit FormatEtc(CLIPFORMAT cfformat, DWORD dwtymed = TYMED_HGLOBAL, DVTARGETDEVICE* pdvtd = nullptr,
        DWORD dwaspect = DVASPECT_CONTENT, LONG index = -1) noexcept : FORMATETC()
    {
        CommonConstruct(cfformat, dwtymed, pdvtd, dwaspect, index);
    }

    explicit FormatEtc(PCWSTR format, DWORD tymed = TYMED_HGLOBAL, DVTARGETDEVICE* pdvtd = nullptr,
         DWORD aspect = DVASPECT_CONTENT, LONG index = -1) noexcept : FORMATETC()
    {
        CommonConstruct(Win32::RegisterClipboardFormat(format), tymed, pdvtd, aspect, index);
    }

    ~FormatEtc()
    {
        Dispose();
    }

    FormatEtc& operator=(const FORMATETC& formatetc)
    {
        if (this == &formatetc)
            return *this;

        Dispose();
        Copy(*this, formatetc);
        return *this;
    }

    explicit FormatEtc(const FormatEtc&) = default;
    explicit FormatEtc(FormatEtc&&) = default;
    FormatEtc& operator=(const FormatEtc&) = delete;
    FormatEtc& operator=(FormatEtc&&) = delete;

    void Dispose() noexcept
    {
        if (ptd)
        {
            CoTaskMemFree(ptd);
            ptd = nullptr;
        }
    }

private:
    void CommonConstruct(CLIPFORMAT cfformat, DWORD dwtymed, DVTARGETDEVICE* pdvtd, DWORD dwaspect, LONG index) noexcept
    {
        cfFormat = cfformat;
        tymed    = dwtymed;
        ptd      = pdvtd;
        dwAspect = dwaspect;
        lindex   = index;
    }

    static void Copy(FORMATETC& destination, const FORMATETC& source)
    {
        DVTARGETDEVICE* targetDevice = CopyTargetDevice(source);
        destination = source;
        destination.ptd = targetDevice;
    }

    static DVTARGETDEVICE* CopyTargetDevice(const FORMATETC& source)
    {
        if (!source.ptd)
            return nullptr;

        auto* ptd = static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(source.ptd->tdSize));
        if (!ptd)
            throw std::bad_alloc();
        memcpy(ptd, source.ptd, source.ptd->tdSize);
        return ptd;
    }
};

} // namespace msf
