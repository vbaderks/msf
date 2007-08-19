//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


DEFINE_GUID(IID_IUIElement, 0xEC6FE84F,0xDC14,0x4FBB,0x88,0x9F,0xEA,0x50,0xFE,0x27,0xFE,0x0F);
DEFINE_GUID(IID_IUICommand, 0x4026DFB9,0x7691,0x4142,0xB7,0x1C,0xDC,0xF0,0x8E,0xA4,0xDD,0x9C);
DEFINE_GUID(IID_IEnumUICommand, 0x869447DA,0x9F84,0x4E2A,0xB9,0x2D,0x00,0x64,0x2D,0xC8,0xA9,0x11);


struct IEnumShellItems : public IUnknown
{
    STDMETHOD(Next)(ULONG celt, IShellItem **rgelt, ULONG *pceltFetched) = 0;
    STDMETHOD(Skip)(ULONG celt) = 0;
    STDMETHOD(Reset)() = 0;
    STDMETHOD(Clone)(IEnumShellItems** ppenum) = 0;
};

#ifndef __IShellItemArray_INTERFACE_DEFINED__
struct IShellItemArray : public IUnknown
{
    STDMETHOD(BindToHandler)(IBindCtx* pbc, REFGUID rbhid, REFIID riid, void** ppvOut) = 0;
    STDMETHOD(GetAttrributes)(int nEnum, DWORD dwRequested, DWORD* pdwResult) = 0;
    STDMETHOD(GetCount)(UINT* pCount) = 0;
    STDMETHOD(GetItemAt)(UINT nIndex, IShellItem** ppItem) = 0;
    STDMETHOD(EnumItems)(IEnumShellItems**) = 0;
};
#endif

struct IUIElement : public IUnknown
{
    // IUIElement methods
    STDMETHOD(get_Name)(IShellItemArray* pItemArray, BSTR* bstrName) = 0;
    STDMETHOD(get_Icon)(IShellItemArray* pItemArray, BSTR* bstrName) = 0;
    STDMETHOD(get_Tooltip)(IShellItemArray* pItemArray, BSTR* bstrName) = 0;
};


struct SFVM_WEBVIEW_CONTENT_DATA
{
    long l1;
    long l2;
    IUIElement*  pUIElement1;
    IUIElement*  pUIElement2;
    IEnumIDList* pEnum;
};
