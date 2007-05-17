//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/extractimageimpl.h"
#include "shellfolderclsid.h"
#include "resource.h"


// TODO: Use the Create GUID tool to create a new UUID
class DECLSPEC_UUID("74B91FD3-61EF-4bee-B3EF-30AFE328F070") CExtractImage;


class ATL_NO_VTABLE CExtractImage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExtractImage, &__uuidof(CExtractImage)>,
	public IExtractImageImpl<CExtractImage>
{
public:
	BEGIN_COM_MAP(CExtractImage)
		COM_INTERFACE_ENTRY(IPersistFile)
		COM_INTERFACE_ENTRY(IExtractImage)
		COM_INTERFACE_ENTRY(IExtractImage2)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw()
	{
		// TODO: Add a description and an extension.
		return IExtractImageImpl<CExtractImage>::UpdateRegistry(IDR_EXTRACTIMAGE, bRegister,
			L"<description>", __uuidof(CShellFolder), L"<.extension>");
	}


	HBITMAP CreateImage(const SIZE& /*size*/, DWORD /*dwRecClrDepth*/, DWORD /*dwFlags*/)
	{
		return NULL; // TODO: create and return a hbitmap.
	}
};

OBJECT_ENTRY_AUTO(__uuidof(CExtractImage), CExtractImage)
