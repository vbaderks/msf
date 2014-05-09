//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "updateregistry.h"
#include "catchhandler.h"
#include <shlobj.h>


namespace MSF
{

template <typename T>
class ATL_NO_VTABLE IExtractImageImpl :
    public IPersistFile,
    public IExtractImage2
{
public:
    // Registration function to register the extension.
    static HRESULT WINAPI UpdateRegistry(UINT nResId, BOOL bRegister,
        PCWSTR szDescription, const CLSID& clsidShellFolder, PCWSTR szExtension) throw()
    {
        return UpdateRegistryFromResource(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), clsidShellFolder, szExtension);
    }


    // Registration function to register the COM object + the root extension.
    static HRESULT WINAPI UpdateRegistryForRootExt(UINT nResId, BOOL bRegister,
        PCWSTR szDescription, PCWSTR szRootExt) throw()
    {
        return UpdateRegistryForRootExt(nResId, bRegister,
            szDescription, T::GetObjectCLSID(), szRootExt);
    }


    // All-in-one registration function for 1 extenstion, call 'ForExt' to register
    // aditional functions.
    static HRESULT WINAPI UpdateRegistry(UINT nResIdRoot, UINT nResIdExt, BOOL bRegister,
        PCWSTR szDescription, PCWSTR szRootExt, PCWSTR szExtension) throw()
    {
        return ::UpdateRegistry(nResIdRoot, nResIdExt, bRegister,
            szDescription, T::GetObjectCLSID(), szRootExt, szExtension);
    }


    IExtractImageImpl() :
        _hbitmap(nullptr)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::IExtractImageImpl (instance=%p)\n", this);
    }


    ~IExtractImageImpl()
    {
        ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::~IExtractImageImpl (instance=%p)\n", this);

        Dispose();
    }


    // IPersistFile
    STDMETHOD(GetClassID)(__RPC__out CLSID* pClassID)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::GetClassID\n");

        if (!pClassID)
            return E_POINTER;

        *pClassID = T::GetObjectCLSID();
        return S_OK;
    }


    STDMETHOD(IsDirty)()
    {
        MSF_TRACENOTIMPL(L"IExtractImageImpl::IsDirty");
    }


    STDMETHOD(Save)(LPCOLESTR, BOOL)
    {
        MSF_TRACENOTIMPL(L"IExtractImageImpl::Save");
    }


    STDMETHOD(SaveCompleted)(LPCOLESTR)
    {
        MSF_TRACENOTIMPL(L"IExtractImageImpl::SaveCompleted");
    }


    STDMETHOD(GetCurFile)(LPOLESTR*)
    {
        MSF_TRACENOTIMPL(L"IExtractImageImpl::GetCurFile");
    }


    STDMETHOD(Load)(LPCOLESTR wszFilename, DWORD dwMode)
    {
        (dwMode); // unused in release.

        ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::Load (instance=%p, mode=%d, filename=%s)\n", this, dwMode, wszFilename);
        try
        {
            _strFilename = CW2T(wszFilename);
            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    // IExtractImage
    STDMETHOD(GetLocation)(LPWSTR pszPathBuffer, DWORD cch, __RPC__inout_opt DWORD* pdwPriority,
                           __RPC__in const SIZE* psize, DWORD dwRecClrDepth, __RPC__inout DWORD* pdwFlags)
    {
        try
        {
            ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::GetLocation (instance=%p, flags=%d)\n", this, *pdwFlags);

            Dispose();
            _hbitmap = static_cast<T*>(this)->CreateImage(*psize, dwRecClrDepth, *pdwFlags);

            lstrcpynW(pszPathBuffer, CT2W(static_cast<T*>(this)->GetPathBuffer()), static_cast<int>(cch));
            *pdwFlags |= IEIFLAG_CACHE;

            //  Note: The SDK docs are unclear if it passing a NULL pointer is allowed.
            if (pdwPriority)
            {
                *pdwPriority = IEIT_PRIORITY_NORMAL;
            }

            return S_OK;
        }
        MSF_COM_CATCH_HANDLER()
    }


    STDMETHOD(Extract)(__RPC__deref_out_opt HBITMAP* phBmpThumbnail)
    {
        ATLTRACE2(atlTraceCOM, 0, "IExtractImageImpl::Extract (instance=%p)\n", this);

        if (!_hbitmap)
            return E_FAIL;

        *phBmpThumbnail = _hbitmap;
        _hbitmap = nullptr;

        return S_OK;
    }


    // IExtractImage2
    STDMETHOD(GetDateStamp)(__RPC__in FILETIME* pDateStamp)
    {
        ATLTRACE2(atlTraceCOM, 0, L"IExtractImageImpl::GetDateStamp (instance=%p, pdatastampe=%p)\n", this, pDateStamp);

        if (!pDateStamp)
            return E_POINTER;

        WIN32_FILE_ATTRIBUTE_DATA fileattributedata;
        if (!GetFileAttributesEx(static_cast<T*>(this)->GetPathBuffer(), GetFileExInfoStandard, &fileattributedata))
            return E_FAIL;
        
        *pDateStamp = fileattributedata.ftLastWriteTime;
        return S_OK;
    }


    // Purpose: returns the identifier for the file that is used to 
    //          extract the image from. Can be used to optimize
    //          cases were the image is extracted from a different file 
    //          then set by IPersistFile::Load.
    CString GetPathBuffer()
    {
        return _strFilename;
    }

protected:

    void Dispose()
    {
        if (_hbitmap != NULL)
        {
            ATLVERIFY(DeleteObject(_hbitmap));
            _hbitmap = NULL;
        }
    }

    // member variables.
    CString _strFilename;
    HBITMAP _hbitmap;
};

} // namespace MSF
