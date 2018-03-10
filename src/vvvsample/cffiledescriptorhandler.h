//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "vvvitem.h"
#include <msf.h>

// Note: owner of the instance of this class must keep passed dataobject alive.
//       This class doesn't do addref to prevent circular referencing.

class CCfFileDescriptorHandler : public msf::ClipboardFormatHandler
{
public:
    explicit CCfFileDescriptorHandler(IDataObject* pdataobjectOuter) :
        ClipboardFormatHandler(CFSTR_FILEDESCRIPTOR, true, false),
        m_pdataobject(pdataobjectOuter)
    {
    }

    ~CCfFileDescriptorHandler() = default;
    CCfFileDescriptorHandler(const CCfFileDescriptorHandler&) = delete;
    CCfFileDescriptorHandler(CCfFileDescriptorHandler&&) = delete;
    CCfFileDescriptorHandler& operator=(const CCfFileDescriptorHandler&) = delete;
    CCfFileDescriptorHandler& operator=(CCfFileDescriptorHandler&&) = delete;

    void GetData(const FORMATETC&, STGMEDIUM& medium) const override
    {
        msf::CCfShellIdList cfshellidlist(m_pdataobject);

        const size_t size = sizeof(FILEGROUPDESCRIPTOR) +
            (cfshellidlist.GetItemCount() * sizeof(FILEDESCRIPTOR));

        const HGLOBAL hg = msf::GlobalAllocThrow(size);
        auto* pfgd = static_cast<FILEGROUPDESCRIPTOR*>(hg);
        pfgd->cItems = static_cast<UINT>(cfshellidlist.GetItemCount());

        for (unsigned int i = 0; i < pfgd->cItems; ++i)
        {
            FILEDESCRIPTOR& fd = pfgd->fgd[i];

            fd.dwFlags = FD_FILESIZE;
            fd.nFileSizeHigh = 0;

            const VVVItem vvvitem(cfshellidlist.GetItem(i));

            fd.nFileSizeLow = min(vvvitem.GetSize(), MAX_VVV_ITEM_SIZE);
            if (lstrcpyn(fd.cFileName, vvvitem.GetDisplayName().c_str(), MAX_PATH))
                _com_raise_error(E_FAIL);
        }

        msf::StorageMedium::SetHGlobal(medium, hg);
    }

private:

    IDataObject* m_pdataobject;
};
