﻿//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cf_handler.h"
#include "pidl.h"
#include "util.h"
#include "stg_medium.h"


namespace msf
{

// Note: handler expect that pidls are alive as long as handler is used.

class CCfShellIdListHandler : public ClipboardFormatHandler
{
public:
    CCfShellIdListHandler(LPCITEMIDLIST pidlFolder, std::vector<ItemIDList>& pidls)
        :
        ClipboardFormatHandler(CFSTR_SHELLIDLIST, true, false),
        m_pidlFolder(pidlFolder),
        m_pidls(pidls)
    {
    }

    CCfShellIdListHandler& operator=(const CCfShellIdListHandler&) = delete;

    [[nodiscard]] HRESULT Validate(const FORMATETC& formatetc) const override
    {
        if (formatetc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!IsBitSet(formatetc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (formatetc.lindex != -1)
            return DV_E_LINDEX;

        return S_OK;
    }


    void GetData(const FORMATETC&, STGMEDIUM& stgmedium) const override
    {
        const uint32_t sizeHeader = static_cast<uint32_t>(sizeof(CIDA) + (sizeof(uint32_t) * m_pidls.size()));

        uint32_t size = sizeHeader + m_pidlFolder.GetSize();

        for (auto it = m_pidls.begin(); it != m_pidls.end(); ++it)
        {
            size += it->GetSize();
        }

        StorageMedium medium(GlobalAllocThrow(size));

        CIDA* pcida = static_cast<CIDA*>(medium.GetHGlobal());

        pcida->cidl = static_cast<uint32_t>(m_pidls.size());

        uint32_t offset = sizeHeader;
        offset += AddPidlToCida(pcida, m_pidlFolder, 0, offset);

        for (uint32_t i = 0; i < m_pidls.size(); ++i)
        {
            offset += AddPidlToCida(pcida, m_pidls[i], i + 1, offset);
        }

        medium.Detach(stgmedium);
    }

private:
    unsigned int AddPidlToCida(CIDA* pcida, const ItemIDList& pidl, uint32_t index, uint32_t offset) const noexcept
    {
        pcida->aoffset[index] = offset;

        const unsigned int size = pidl.GetSize();
        memcpy(reinterpret_cast<BYTE*>(pcida) + offset, pidl.get(), size);
        return size;
    }

    ItemIDList m_pidlFolder;
    std::vector<ItemIDList>& m_pidls;
};

} // end of msf namespace

