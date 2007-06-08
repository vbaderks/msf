//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "cfhandler.h"
#include "util.h"
#include "performeddropeffectsink.h"

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

		ATLTRACE2(atlTraceCOM, 0, _T("CCfPerformedDropEffectHandler::SetData (dwEffect=%p)\n"), m_dwEffect);

		if (IsShell5OrHigher())
		{
			if (m_dwEffect == DROPEFFECT_MOVE)
			{
				NotifySink();
			}
		}
		else
		{
			// Note: Windows 98 will pass DROPEFFECT_NONE instead of DROPEFFECT_MOVE
			// See SDK documentation CFSTR_LOGICALPERFORMEDDROPEFFECT
			if (m_dwEffect == DROPEFFECT_NONE)
			{
				NotifySink();
			}
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
		if (m_rperformeddropeffectsink != NULL)
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
