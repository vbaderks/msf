//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "dataobjectptr.h"
#include "stgmedium.h"
#include "globallock.h"
#include "formatetc.h"
#include "util.h"


namespace MSF
{

class CCfShellIdList
{
public:
    CCfShellIdList(IDataObjectPtr dataobject)
    {
        CFormatEtc formatetc(RegisterCf(CFSTR_SHELLIDLIST));

        dataobject.GetData(formatetc, m_stgmedium);

        m_globallock.Attach(m_stgmedium.hGlobal);
    }


    ~CCfShellIdList() throw()
    {
        m_globallock.Dispose();
    }


    bool empty() const throw()
    {
        return GetItemCount() == 0;
    }


    unsigned int GetItemCount() const throw()
    {
        return m_globallock.get()->cidl;
    }


    const ITEMIDLIST* GetItem(UINT nIdex) const throw()
    {
        const CIDA* pcida = m_globallock.get();

        return reinterpret_cast<const ITEMIDLIST*>
            (reinterpret_cast<const BYTE*>(pcida) + pcida->aoffset[nIdex + 1]);
    }

private:

    CCfShellIdList& operator=(const CCfShellIdList&); // not implemented by design.

    CGlobalLock<CIDA> m_globallock;
    CStgMedium        m_stgmedium;
};

} // namespace MSF
