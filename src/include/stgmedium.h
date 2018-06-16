//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "util.h"
#include "globallock.h"
#include <objidl.h>

namespace msf {

// A StorageMedium class is owner of the stgmedium.

/// <summary>Smart extender class for Windows SDK STGMEDIUM struct.</summary>
/// <remarks>
/// The StorageMedium class extends STGMEDIUM with typical constructors and a
/// destructor that will clean up any dynamic allocated memory.
/// </remarks>
class StorageMedium final : public STGMEDIUM
{
public:
    static HGLOBAL GlobalClone(HGLOBAL hglobIn)
    {
        if (!hglobIn)
            return nullptr;

        util::GlobalLock<void> globallock(hglobIn);

        const size_t cb = GlobalSize(hglobIn);
        const HGLOBAL hglobOut = GlobalAllocThrow(cb);
        CopyMemory(hglobOut, globallock.get(), cb);
        return hglobOut;
    }

    static void SetHGlobal(STGMEDIUM& stgmedium, HGLOBAL hglobal) noexcept
    {
        stgmedium.tymed          = TYMED_HGLOBAL;
        stgmedium.hGlobal        = hglobal;
        stgmedium.pUnkForRelease = nullptr;
    }

    StorageMedium() noexcept
        : STGMEDIUM()
    {
    }

    explicit StorageMedium(HGLOBAL hg) noexcept
        : STGMEDIUM()
    {
        SetHGlobal(*this, hg);
    }

    // Purpose: passed in STGMEDIUM will be owned after a bitwise copy.
    explicit StorageMedium(const STGMEDIUM& stgmedium) noexcept
        : STGMEDIUM()
    {
        *static_cast<STGMEDIUM*>(this) = stgmedium;
    }

    ~StorageMedium()
    {
        if (tymed != TYMED_NULL)
        {
            ReleaseStgMedium(this);
        }
    }

    StorageMedium(const StorageMedium&) = delete;
    StorageMedium(StorageMedium&&) = delete;
    StorageMedium& operator=(const StorageMedium&) = delete;
    StorageMedium& operator=(StorageMedium&&) = delete;

    HGLOBAL GetHGlobal() const noexcept
    {
        ATLASSERT(tymed == TYMED_HGLOBAL && "Can only get hglobal if correct type");
        return hGlobal;
    }

    void Detach() noexcept
    {
        tymed = TYMED_NULL;
    }

    void Detach(STGMEDIUM& stgmedium) noexcept
    {
        stgmedium = *this;
        Detach();
    }

    StorageMedium& operator = (STGMEDIUM& stgmedium) noexcept
    {
        Detach();

        *static_cast<STGMEDIUM*>(this) = stgmedium;
        stgmedium.tymed = TYMED_NULL;
        stgmedium.pUnkForRelease = nullptr;

        return *this;
    }

    void CopyTo(STGMEDIUM& stgmedium) const
    {
        stgmedium.tymed          = tymed;
        stgmedium.pUnkForRelease = pUnkForRelease;

        switch (tymed)
        {
        case TYMED_HGLOBAL:
            stgmedium.hGlobal = GlobalClone(GetHGlobal());
            break;

        case TYMED_ISTREAM:
            stgmedium.pstm = pstm;
            stgmedium.pstm->AddRef();
            break;

        case TYMED_ISTORAGE:
            stgmedium.pstg = pstg;
            stgmedium.pstg->AddRef();
            break;

        default:
            ATLASSERT(!"other types are currently not supported.");
            break;
        }
    }
};

} // end msf namespace
