//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msfbase.h"
#include "stgmedium.h"
#include "globallock.h"
#include "formatetc.h"
#include "util.h"
#include "smartptr/dataobjectptr.h"

namespace msf {

/// <summary>Wraps a data object as a collection of ITEMIDLIST pointers.</summary>
class CCfShellIdList final
{
public:
    explicit CCfShellIdList(IDataObjectPtr dataObject)
    {
        const FormatEtc formatEtc(Win32::RegisterClipboardFormat(CFSTR_SHELLIDLIST));
        dataObject.GetData(formatEtc, m_medium);
        m_globalLock.Attach(m_medium.hGlobal);
    }

    ~CCfShellIdList()
    {
        m_globalLock.Dispose();
    }

    CCfShellIdList(const CCfShellIdList& other) = delete;
    CCfShellIdList(CCfShellIdList&& other) = delete;
    CCfShellIdList& operator=(const CCfShellIdList&) = delete;
    CCfShellIdList& operator=(CCfShellIdList&& other) = delete;

    bool IsEmpty() const noexcept
    {
        return GetItemCount() == 0;
    }

    size_t GetItemCount() const noexcept
    {
        return static_cast<size_t>(m_globalLock.get()->cidl);
    }

    PCIDLIST_RELATIVE GetItem(size_t nIdex) const noexcept
    {
        const CIDA* pcida = m_globalLock.get();
        return reinterpret_cast<PCIDLIST_RELATIVE>
            (reinterpret_cast<const BYTE*>(pcida) + pcida->aoffset[nIdex + 1]);
    }

private:
    util::GlobalLock<CIDA> m_globalLock;
    StorageMedium m_medium;
};

} // namespace msf
