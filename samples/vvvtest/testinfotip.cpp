//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"

#include "testinfotip.h"

#include "../include/infotipptr.h"
#include "../include/util.h"
#include "../include/testutil.h"
#include "../include/shlobjhidden.h"
#include "../include/olestring.h"
#include "../vvvsample/infotipclsid.h"


namespace
{

CStringW TestLoadInfoTip(PCWSTR wszFilename)
{
    MSF::IInfoTipPtr infotip(__uuidof(CInfoTip));

    return infotip.GetInfoTip(MSF::GetModuleDirectoryW() + wszFilename);
}

}


void TestInfoTip::Sanity()
{
    TestLoadInfoTip(L"sample1.vvv");
}


void TestInfoTip::FileNotFound()
{
    try
    {
        TestLoadInfoTip(L"filenotfound.vvv");
    }
    catch (const _com_error&)
    {
        // expect failed HRESULT.
        return;
    }

    RaiseExceptionIfFailed(E_FAIL);
}


void TestInfoTip::IncorrectType()
{
    try
    {
        TestLoadInfoTip(L"sample.yyy");
    }
    catch (const _com_error&)
    {
        // expect failed HRESULT.
        return;
    }

    RaiseExceptionIfFailed(E_FAIL);
}


void TestInfoTip::NotImplementedFunctions()
{
    MSF::IPersistFilePtr persistfile(__uuidof(CInfoTip));

    CLSID classId;
    HRESULT hr = persistfile->GetClassID(&classId);
    RaiseExceptionIfImplemented(hr);

    hr = persistfile->IsDirty();
    RaiseExceptionIfImplemented(hr);

    hr = persistfile->Save(L"dummy", FALSE);
    RaiseExceptionIfImplemented(hr);

    hr = persistfile->SaveCompleted(L"dummy");
    RaiseExceptionIfImplemented(hr);

    COleString olestring;
    hr = persistfile->GetCurFile(olestring.GetAddress());
    RaiseExceptionIfImplemented(hr);

    MSF::IQueryInfoPtr queryinfo(__uuidof(CInfoTip));
    
    DWORD dw;
    hr = queryinfo->GetInfoFlags(&dw);
    RaiseExceptionIfImplemented(hr);
}
