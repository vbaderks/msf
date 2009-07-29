//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "../include/extractimageimpl.h"
#include "extractimageclsid.h"
#include "shellfolderclsid.h"
#include "resource.h"


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
        // TODO: update to new reg model.
		return IExtractImageImpl<CExtractImage>::UpdateRegistry(IDR_EXTRACTIMAGE, bRegister,
			L"Sample ShellExtension ExtractImage", __uuidof(CShellFolder), wszVVVExtension);
	}


	HBITMAP CreateImage(const SIZE& size, DWORD dwRecClrDepth, DWORD /*dwFlags*/)
	{
		// TODO: fix this function
		HBITMAP hbitmap = CreateBitmap(size.cx, size.cy, 1, dwRecClrDepth, NULL);

		HDC hdc = CreateCompatibleDC(NULL);

		HGDIOBJ hgdiobjectOriginal = SelectObject(hdc, hbitmap);

		HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

		RECT rect = {0, 0, size.cx, size.cx};
		FillRect(hdc, &rect, hbrush);

		DeleteObject(hbrush);
		hgdiobjectOriginal = SelectObject(hdc, hgdiobjectOriginal);
		DeleteDC(hdc);

		return hbitmap;
	}
};

#if 0
HDC dc= CreateCompatibleDC(NULL);
BITMAPINFO i;
ZeroMemory( &i.bmiHeader, sizeof(BITMAPINFOHEADER) );
i.bmiHeader.biWidth=5;     // Set size you need
i.bmiHeader.biHeight=5;    // Set size you need
i.bmiHeader.biPlanes=1;
i.bmiHeader.biBitCount=24; // Can be 8, 16, 32 bpp or
                           // other number
i.bmiHeader.biSizeImage=0;
i.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
i.bmiHeader.biClrUsed= 0;
i.bmiHeader.biClrImportant= 0;
VOID *pvBits;
HBITMAP hbmp= CreateDIBSection( dc,
                                &i,
                                DIB_RGB_COLORS,
                                &pvBits,
                                NULL,
                                0 );

// --------------------------

	HBITMAP Create_DIB_Sec( int cxImage, int cyImage,
        int nBitsPerPixel, void **ppImageBits )
{
    HDC hDC = GetDC( NULL );
 
    BITMAPINFO BitmapInfo;
    ZeroMemory( &BitmapInfo, sizeof(BITMAPINFO) );
    BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = cxImage;
    BitmapInfo.bmiHeader.biHeight = cyImage;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = nBitsPerPixel;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    BitmapInfo.bmiHeader.biSizeImage =
        ( 4 * ((m_cxImage * nBitsPerPixel + 31) / 32) ) * m_cyImage;
 
    HBITMAP hBitmap = CreateDIBSection( hDC,
        &BitmapInfo, DIB_RGB_COLORS,
        ppImageBits, NULL, 0 );
 
    ReleaseDC( NULL, hDC );
 
    return hBitmap;
}

#endif

OBJECT_ENTRY_AUTO(__uuidof(CExtractImage), CExtractImage)
