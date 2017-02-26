//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../include/cfhandler.h"
#include "../include/cfshellidlist.h"

// Note: owner of the instance of this class must keep passed dataobject alive.
//       This class doesn't do addref to prevent circulair referencing.

class CCfFileDescriptorHandler : public CCfHandler
{
public:
    explicit CCfFileDescriptorHandler(IDataObject* pdataobjectOuter) :
        CCfHandler(CFSTR_FILEDESCRIPTOR, true, false),
        m_pdataobject(pdataobjectOuter)
    {
    }

    CCfFileDescriptorHandler& operator=(const CCfFileDescriptorHandler&) = delete;

    void GetData(const FORMATETC&, STGMEDIUM& medium) const override
    {
        CCfShellIdList cfshellidlist(m_pdataobject);

        size_t size = sizeof(FILEGROUPDESCRIPTOR) +
            (cfshellidlist.GetItemCount() * sizeof(FILEDESCRIPTOR));

        HGLOBAL hg = GlobalAllocThrow(size);

        FILEGROUPDESCRIPTOR* pfgd = static_cast<FILEGROUPDESCRIPTOR*>(hg);

        pfgd->cItems = static_cast<UINT>(cfshellidlist.GetItemCount());

        for (unsigned int i = 0; i < pfgd->cItems; ++i)
        {
            FILEDESCRIPTOR& fd = pfgd->fgd[i];

            fd.dwFlags = FD_FILESIZE;
            fd.nFileSizeHigh = 0;

            VVVItem vvvitem(cfshellidlist.GetItem(i));

            fd.nFileSizeLow = min(vvvitem.GetSize(), MAX_VVV_ITEM_SIZE);
            if (lstrcpyn(fd.cFileName, vvvitem.GetDisplayName(), MAX_PATH))
                _com_raise_error(E_FAIL);
        }

        CStgMedium::SetHGlobal(medium, hg);
    }

private:

    IDataObject* m_pdataobject;
};
