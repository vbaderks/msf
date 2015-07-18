//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cfhandler.h"
#include "performeddropeffectsink.h"
#include "globallock.h"
#include "stgmedium.h"
#include "util.h"

namespace MSF
{

class CCfPerformedDropEffectHandler : public CCfHandler
{
public:
    CCfPerformedDropEffectHandler(IPerformedDropEffectSink* pperformeddropeffectsink, IDataObject* pdataobject) :
        CCfHandler(CFSTR_PERFORMEDDROPEFFECT, false, true),
        m_rperformeddropeffectsink(pperformeddropeffectsink),
        m_pdataobject(pdataobject),
        m_dwEffect(DROPEFFECT_NONE)
    {
    }

    virtual void SetData(const FORMATETC& formatetc, STGMEDIUM& stgmedium, bool bRelease)
    {
        ATLASSERT(IsValid(formatetc, stgmedium) && "Passed invalid arguments");
        (formatetc); // not used in release.

        CGlobalLock<DWORD> globallock(stgmedium.hGlobal);

        m_dwEffect = *globallock.get();

        ATLTRACE2(atlTraceCOM, 0, L"CCfPerformedDropEffectHandler::SetData (dwEffect=%p)\n", m_dwEffect);

        if (m_dwEffect == DROPEFFECT_MOVE)
        {
            NotifySink();
        }

        if (bRelease)
        {
            ReleaseStgMedium(&stgmedium);
        }
    }

    virtual void GetData(const FORMATETC&, STGMEDIUM& stgmedium) const
    {
        CStgMedium medium(GlobalAllocThrow(sizeof(DWORD)));
        *static_cast<DWORD*>(medium.GetHGlobal()) = m_dwEffect;
        medium.Detach(stgmedium);
    }

private:
    CCfPerformedDropEffectHandler& operator=(const CCfPerformedDropEffectHandler&); // not implemented.

    void NotifySink()
    {
        if (m_rperformeddropeffectsink)
        {
            m_rperformeddropeffectsink->OnDeleteAfterPaste(m_pdataobject);
        }
    }

    // member variables
    CComPtr<IPerformedDropEffectSink> m_rperformeddropeffectsink;
    IDataObject*                      m_pdataobject;
    DWORD                             m_dwEffect;
};

} // end of MSF namespace
