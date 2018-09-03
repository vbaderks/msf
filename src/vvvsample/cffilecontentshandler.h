//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software license.
//
#pragma once

#include "vvvitem.h"

#include <msf.h>
#include <memory>

// Note: owner of the instance of this class must keep passed data object alive.
//       This class doesn't do increase the reference count to prevent circular referencing.

class CfFileContentsHandler : public msf::ClipboardFormatHandler
{
public:
    explicit CfFileContentsHandler(IDataObject* dataObjectOuter) :
        ClipboardFormatHandler(CFSTR_FILECONTENTS, true, false),
        m_dataObject(dataObjectOuter)
    {
    }

    ~CfFileContentsHandler() = default;
    CfFileContentsHandler(const CfFileContentsHandler&) = delete;
    CfFileContentsHandler(CfFileContentsHandler&&) = delete;
    CfFileContentsHandler& operator=(const CfFileContentsHandler&) = delete;
    CfFileContentsHandler& operator=(CfFileContentsHandler&&) = delete;

    HRESULT Validate(const FORMATETC& formatEtc) const noexcept override
    {
        if (formatEtc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!msf::IsBitSet(formatEtc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (static_cast<UINT>(formatEtc.lindex) >= GetCfShellIdList()->GetItemCount())
            return DV_E_LINDEX;

        return S_OK;
    }

    void GetData(const FORMATETC& formatEtc, STGMEDIUM& medium) const override
    {
        ATLASSERT(SUCCEEDED(Validate(formatEtc)));

        const VVVItem vvvItem(GetCfShellIdList()->GetItem(static_cast<UINT>(formatEtc.lindex)));

        const size_t size = std::min(vvvItem.GetSize(), MAX_VVV_ITEM_SIZE);
        const HGLOBAL hg = msf::GlobalAllocThrow(size);
        ZeroMemory(hg, size);

        msf::StorageMedium::SetHGlobal(medium, hg);
    }

private:
    msf::CCfShellIdList* GetCfShellIdList() const
    {
        if (!m_cfShellIdList)
        {
            m_cfShellIdList = std::make_unique<msf::CCfShellIdList>(m_dataObject);
        }

        return m_cfShellIdList.get();
    }

    IDataObject* m_dataObject;
    mutable std::unique_ptr<msf::CCfShellIdList> m_cfShellIdList;
};
