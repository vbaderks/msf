//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "msf_base.h"
#include "stg_medium.h"
#include "global_lock.h"
#include "format_etc.h"
#include "util.h"
#include "smartptr/dataobjectptr.h"

namespace msf {

/// <summary>Wraps a data object as a collection of ITEMIDLIST pointers.</summary>
class CfShellIdList final
{
public:
    explicit CfShellIdList(IDataObjectPtr dataObject)
    {
        const FormatEtc formatEtc(Win32::RegisterClipboardFormat(CFSTR_SHELLIDLIST));
        dataObject.GetData(formatEtc, m_medium);
        m_globalLock.Attach(m_medium.hGlobal);
    }

    ~CfShellIdList()
    {
        m_globalLock.Dispose();
    }

    CfShellIdList(const CfShellIdList&) = delete;
    CfShellIdList(CfShellIdList&&) = delete;
    CfShellIdList& operator=(const CfShellIdList&) = delete;
    CfShellIdList& operator=(CfShellIdList&&) = delete;

    [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0;
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return static_cast<size_t>(m_globalLock.get()->cidl);
    }

    [[nodiscard]] PCIDLIST_RELATIVE GetItem(size_t index) const noexcept
    {
        const CIDA* item = m_globalLock.get();
        return reinterpret_cast<PCIDLIST_RELATIVE>
            (reinterpret_cast<const BYTE*>(item) + item->aoffset[index + 1]);
    }

private:
    util::GlobalLock<CIDA> m_globalLock;
    StorageMedium m_medium;
};

} // namespace msf
