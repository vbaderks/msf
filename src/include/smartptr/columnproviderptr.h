//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "../msfbase.h"


_COM_SMARTPTR_TYPEDEF(IColumnProvider, __uuidof(IColumnProvider));


namespace MSF
{

class CSHColumnInit : public SHCOLUMNINIT
{
public:
    CSHColumnInit()
    {
        dwFlags = 0; // reserved.
        dwReserved = 0;
        wszFolder[0] = 0;
    }

    CSHColumnInit(PCWSTR pwszFolder)
    {
        dwFlags = 0; // reserved.
        dwReserved = 0;
        lstrcpynW(wszFolder, pwszFolder, MSF_ARRAY_SIZE(wszFolder));
    }
};


class IColumnProviderPtr :
    public ::IColumnProviderPtr
{
public:
    IColumnProviderPtr(const CLSID& clsid, IUnknown* pOuter, DWORD dwClsContext) :
        ::IColumnProviderPtr(clsid, pOuter, dwClsContext)
    {
    }


    IColumnProviderPtr(const CLSID& clsid, wchar_t* pwszFolder = nullptr) :
        ::IColumnProviderPtr(clsid, nullptr, CLSCTX_INPROC_SERVER)
    {
        Initialize(pwszFolder);
    }


    void CreateInstance(const CLSID& rclsid)
    {
        RaiseExceptionIfFailed(__super::CreateInstance(rclsid));
    }


    void Initialize(wchar_t* pwszFolder = nullptr)
    {
        CSHColumnInit shcolumninit(pwszFolder);

        RaiseExceptionIfFailed(GetInterfacePtr()->Initialize(&shcolumninit));
    }


    bool GetColumnInfo(DWORD dwIndex, SHCOLUMNINFO& columninfo)
    {
        HRESULT hr = GetInterfacePtr()->GetColumnInfo(dwIndex, &columninfo);
        RaiseExceptionIfFailed(hr);

        return hr == S_OK;
    }


    // Helper that retrieves id as return or throws.
    SHCOLUMNID GetColumnId(DWORD dwIndex)
    {
        SHCOLUMNINFO columninfo;

        RaiseExceptionIf(!GetColumnInfo(dwIndex, columninfo));

        return columninfo.scid;
    }


    unsigned long GetColumnCount()
    {
        DWORD dwIndex = 0;
        SHCOLUMNINFO columninfo;

        while (GetColumnInfo(dwIndex, columninfo))
        {
            ++dwIndex;
        }

        return dwIndex;
    }


    bool GetItemData(PCWSTR wszFilename, SHCOLUMNID& scid, VARIANT& varData, ULONG dwflags = 0)
    {
        CShColumnData shcolumndata(wszFilename, dwflags);

        HRESULT hr = GetInterfacePtr()->GetItemData(&scid, &shcolumndata, &varData);
        RaiseExceptionIfFailed(hr);

        return hr == S_OK;
    }


    _variant_t GetItemData(PCWSTR wszFilename, SHCOLUMNID& scid, ULONG dwflags = 0)
    {
        _variant_t varData;
        RaiseExceptionIf(!GetItemData(wszFilename, scid, varData, dwflags));

        return varData;
    }

private:
    class CShColumnData : public SHCOLUMNDATA
    {
    public:
        CShColumnData(PCWSTR wszFilename, ULONG dwflags = 0, DWORD dwfileattributes = FILE_ATTRIBUTE_NORMAL)
        {
            dwFlags = dwflags;
            dwFileAttributes = dwfileattributes;
            dwReserved = 0;
            lstrcpynW(wszFile, wszFilename, MSF_ARRAY_SIZE(wszFile));
            wcscpy(wszFile, wszFilename);
            pwszExt = wcsrchr(wszFile, '.');
        }
    };
};

} // end MSF namespace
