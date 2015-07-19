//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include <objidl.h>
#include "util.h"
#include "globallock.h"

namespace MSF
{

// A CStgMedium class is owner of the stgmedium. 

/// <summary>Smart extender class for Windows SDK STGMEDIUM struct.</summary>
/// <remarks>
/// The CStgMedium class extends STGMEDIUM with typical constructors and a 
/// destructor that will clean up any dynamic allocated memory.
/// </remarks>
class CStgMedium : public STGMEDIUM
{
public:
    static HGLOBAL GlobalClone(HGLOBAL hglobIn)
    {
        if (!hglobIn)
            return nullptr;

        CGlobalLock<void> globallock(hglobIn);

        size_t cb = GlobalSize(hglobIn);
        HGLOBAL hglobOut = GlobalAllocThrow(cb);
        CopyMemory(hglobOut, globallock.get(), cb);
        return hglobOut;
    }


    static void SetHGlobal(STGMEDIUM& stgmedium, HGLOBAL hglobal) MSF_NOEXCEPT
    {
        stgmedium.tymed          = TYMED_HGLOBAL;
        stgmedium.hGlobal        = hglobal;
        stgmedium.pUnkForRelease = nullptr;
    }


    CStgMedium() MSF_NOEXCEPT
    {
        tymed = TYMED_NULL;
        pUnkForRelease = nullptr;
    }


    CStgMedium(HGLOBAL hg) MSF_NOEXCEPT
    {
        SetHGlobal(*this, hg);
    }


    // Purpose: passed in STGMEDIUM will be owned after a bitwise copy.
    CStgMedium(const STGMEDIUM& stgmedium) MSF_NOEXCEPT
    {
        *static_cast<STGMEDIUM*>(this) = stgmedium;
    }


    ~CStgMedium() MSF_NOEXCEPT
    {
        if (tymed != TYMED_NULL)
        {
            ReleaseStgMedium(this);
        }
    }


    HGLOBAL GetHGlobal() const MSF_NOEXCEPT
    {
        ATLASSERT(tymed == TYMED_HGLOBAL && "Can only get hglobal if correct type");
        return hGlobal;
    }


    void Detach(STGMEDIUM& stgmedium) MSF_NOEXCEPT
    {
        stgmedium = *this;
        Detach();
    }


    void Detach() MSF_NOEXCEPT
    {
        tymed = TYMED_NULL;
    }


    CStgMedium& operator = (STGMEDIUM& stgmedium)
    {
        Detach();

        *static_cast<STGMEDIUM*>(this) = stgmedium;
        stgmedium.tymed = TYMED_NULL;
        stgmedium.pUnkForRelease = nullptr;

        return *this;
    }


    void CopyTo(STGMEDIUM& stgmedium)
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

} // end MSF namespace
