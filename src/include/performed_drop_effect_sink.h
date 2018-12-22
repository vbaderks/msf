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
    virtual void OnDeleteAfterPaste(IDataObject* dataObject) = 0;

    IPerformedDropEffectSink(const IPerformedDropEffectSink&) = delete;
    IPerformedDropEffectSink(IPerformedDropEffectSink&&) = delete;
    IPerformedDropEffectSink& operator=(const IPerformedDropEffectSink&) = delete;
    IPerformedDropEffectSink& operator=(IPerformedDropEffectSink&&) = delete;

protected:
    IPerformedDropEffectSink() = default;
    ~IPerformedDropEffectSink() = default;
};

} // end of msf namespace
