//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "vvvitem.h"

#include "../include/cfhandler.h"
#include "../include/cfshellidlist.h"
#include "../include/util.h"

#include <memory>

// Note: owner of the instance of this class must keep passed dataobject alive.
//       This class doesn't do addref to prevent circulair referencing.

class CCfFileContentsHandler : public CCfHandler
{
public:
    CCfFileContentsHandler(IDataObject* pdataobjectOuter) :
        CCfHandler(CFSTR_FILECONTENTS, true, false),
        m_pdataobject(pdataobjectOuter)
    {
    }


    virtual HRESULT Validate(const FORMATETC& formatetc) const
    {
        if (formatetc.dwAspect != DVASPECT_CONTENT)
            return DV_E_DVASPECT;

        if (!IsBitSet(formatetc.tymed, TYMED_HGLOBAL)) 
            return DV_E_TYMED;

        if (static_cast<UINT>(formatetc.lindex) >= GetCfShellIdList()->GetItemCount())
            return DV_E_LINDEX;

        return S_OK;
    }


    virtual void GetData(const FORMATETC& formatetc, STGMEDIUM& medium) const
    {
        ATLASSERT(SUCCEEDED(Validate(formatetc)));

        CVVVItem vvvitem(GetCfShellIdList()->GetItem(static_cast<UINT>(formatetc.lindex)));

        size_t size = min(vvvitem.GetSize(), MAX_VVV_ITEM_SIZE);
        HGLOBAL hg = GlobalAllocThrow(size);
        ZeroMemory(hg, size);

        CStgMedium::SetHGlobal(medium, hg);
    }

private:

    CCfFileContentsHandler& operator=(const CCfFileContentsHandler&); // not implemented.

    CCfShellIdList* GetCfShellIdList() const
    {
        if (!m_qcfshellidlist)
        {
            m_qcfshellidlist = make_unique<CCfShellIdList>(m_pdataobject);
        }

        return m_qcfshellidlist.get();
    }

    IDataObject* m_pdataobject;
    mutable unique_ptr<CCfShellIdList> m_qcfshellidlist;
};
