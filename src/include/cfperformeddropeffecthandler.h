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

namespace msf
{

class CCfPerformedDropEffectHandler : public ClipboardFormatHandler
{
public:
    CCfPerformedDropEffectHandler(IPerformedDropEffectSink* pperformeddropeffectsink, IDataObject* pdataobject) :
        ClipboardFormatHandler(CFSTR_PERFORMEDDROPEFFECT, false, true),
        m_rperformeddropeffectsink(pperformeddropeffectsink),
        m_pdataobject(pdataobject),
        m_dwEffect(DROPEFFECT_NONE)
    {
    }

    CCfPerformedDropEffectHandler& operator=(const CCfPerformedDropEffectHandler&) = delete;

    void SetData(const FORMATETC& formatetc, STGMEDIUM& stgmedium, bool bRelease) override
    {
        ATLASSERT(IsValid(formatetc, stgmedium) && "Passed invalid arguments");
        UNREFERENCED_PARAMETER(formatetc); // not used in release.

        util::GlobalLock<DWORD> globallock(stgmedium.hGlobal);

        m_dwEffect = *globallock.get();

        ATLTRACE2(ATL::atlTraceCOM, 0, L"CCfPerformedDropEffectHandler::SetData (dwEffect=%p)\n", m_dwEffect);

        if (m_dwEffect == DROPEFFECT_MOVE)
        {
            NotifySink();
        }

        if (bRelease)
        {
            ReleaseStgMedium(&stgmedium);
        }
    }

    void GetData(const FORMATETC&, STGMEDIUM& stgmedium) const override
    {
        StorageMedium medium(GlobalAllocThrow(sizeof(DWORD)));
        *static_cast<DWORD*>(medium.GetHGlobal()) = m_dwEffect;
        medium.Detach(stgmedium);
    }

private:

    void NotifySink() const
    {
        if (m_rperformeddropeffectsink)
        {
            m_rperformeddropeffectsink->OnDeleteAfterPaste(m_pdataobject);
        }
    }

    // member variables
    ATL::CComPtr<IPerformedDropEffectSink> m_rperformeddropeffectsink;
    IDataObject*                      m_pdataobject;
    DWORD                             m_dwEffect;
};

} // end of msf namespace
