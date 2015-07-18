//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

struct IPerformedDropEffectSink : public IUnknown
{
	virtual void OnDeleteAfterPaste(IDataObject* pdataobject) = 0;
};

} // end of MSF namespace
