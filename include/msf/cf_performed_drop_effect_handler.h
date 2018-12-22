//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "cf_handler.h"
#include "performed_drop_effect_sink.h"
#include "global_lock.h"
#include "stg_medium.h"
#include "util.h"

namespace msf
{

class ClipboardPerformedDropEffectHandler : public ClipboardFormatHandler
{
public:
    ClipboardPerformedDropEffectHandler(IPerformedDropEffectSink* performedDropEffectSink, IDataObject* dataObject) :
        ClipboardFormatHandler(CFSTR_PERFORMEDDROPEFFECT, false, true),
        m_performedDropEffectSink(performedDropEffectSink),
        m_dataObject(dataObject),
        m_effect(DROPEFFECT_NONE)
    {
    }

    ~ClipboardPerformedDropEffectHandler() = default;
    ClipboardPerformedDropEffectHandler(const ClipboardPerformedDropEffectHandler&) = delete;
    ClipboardPerformedDropEffectHandler(ClipboardPerformedDropEffectHandler&&) = delete;
    ClipboardPerformedDropEffectHandler& operator=(const ClipboardPerformedDropEffectHandler&) = delete;
    ClipboardPerformedDropEffectHandler& operator=(ClipboardPerformedDropEffectHandler&&) = delete;

    void SetData([[maybe_unused]] const FORMATETC& formatEtc, STGMEDIUM& storageMedium, bool release) override
    {
        ATLASSERT(IsValid(formatEtc, storageMedium) && "Passed invalid arguments");

        util::GlobalLock<DWORD> globalLock(storageMedium.hGlobal);

        m_effect = *globalLock.get();

        ATLTRACE(L"ClipboardPerformedDropEffectHandler::SetData (m_effect=%p)\n", m_effect);

        if (m_effect == DROPEFFECT_MOVE)
        {
            NotifySink();
        }

        if (release)
        {
            ReleaseStgMedium(&storageMedium);
        }
    }

    void GetData(const FORMATETC&, STGMEDIUM& storageMedium) const override
    {
        StorageMedium medium(GlobalAllocThrow(sizeof(DWORD)));
        *static_cast<DWORD*>(medium.GetHGlobal()) = m_effect;
        medium.Detach(storageMedium);
    }

private:
    void NotifySink() const
    {
        if (m_performedDropEffectSink)
        {
            m_performedDropEffectSink->OnDeleteAfterPaste(m_dataObject);
        }
    }

    // member variables
    ATL::CComPtr<IPerformedDropEffectSink> m_performedDropEffectSink;
    IDataObject*                      m_dataObject;
    DWORD                             m_effect;
};

} // end of msf namespace
