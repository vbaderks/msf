//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "vvvitem.h"

#include <msf.h>
#include <memory>

// Note: owner of the instance of this class must keep passed dataobject alive.
//       This class doesn't do addref to prevent circulair referencing.

class CfFileContentsHandler : public msf::ClipboardFormatHandler
{
public:
    explicit CfFileContentsHandler(IDataObject* dataObjectOuter) :
        ClipboardFormatHandler(CFSTR_FILECONTENTS, true, false),
        m_dataObject(dataObjectOuter)
    {
    }

    CfFileContentsHandler& operator=(const CfFileContentsHandler&) = delete;

    HRESULT Validate(const FORMATETC& formatetc) const noexcept override
    {
        if (formatetc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!msf::IsBitSet(formatetc.tymed, TYMED_HGLOBAL))
            return DV_E_TYMED;

        if (static_cast<UINT>(formatetc.lindex) >= GetCfShellIdList()->GetItemCount())
            return DV_E_LINDEX;

        return S_OK;
    }

    void GetData(const FORMATETC& formatetc, STGMEDIUM& medium) const override
    {
        ATLASSERT(SUCCEEDED(Validate(formatetc)));

        VVVItem vvvitem(GetCfShellIdList()->GetItem(static_cast<UINT>(formatetc.lindex)));

        size_t size = min(vvvitem.GetSize(), MAX_VVV_ITEM_SIZE);
        HGLOBAL hg = msf::GlobalAllocThrow(size);
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
