//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace msf
{

struct IPerformedDropEffectSink : IUnknown
{
    virtual void OnDeleteAfterPaste(IDataObject* pdataobject) = 0;

protected:
    ~IPerformedDropEffectSink() = default;
};

} // end of msf namespace
