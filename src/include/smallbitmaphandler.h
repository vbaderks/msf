//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "custommenuhandler.h"


namespace MSF
{

class CSmallBitmapHandler : public CCustomMenuHandler
{
public:

    // Purpose: small helper class to free managed handle.
    // For more advanced features use bitmap class in WTL or MFC
    class Bitmap
    {
    public:
        explicit Bitmap(HBITMAP handle = nullptr) : m_handle(handle)
        {
        }

        ~Bitmap()
        {
            if (m_handle)
            {
                ATLVERIFY(DeleteObject(m_handle));
            }
        }

        HBITMAP GetHandle() const
        {
            return m_handle;
        }

    private:

        HBITMAP m_handle;
    };


    CSmallBitmapHandler(CString text, UINT resourceID) :
        _text(text),
        _bitmap(LoadBitmap(resourceID))
    {
    }


    CSmallBitmapHandler(UINT textID, UINT resourceID) :
        _text(LoadString(textID)),
        _bitmap(LoadBitmap(resourceID))
    {
    }


    void InitializeItemInfo(CMenuItemInfo& menuItemInfo) override
    {
        menuItemInfo.SetString(_text);
        menuItemInfo.SetCheckMarkBmps(nullptr, _bitmap.GetHandle());
    }

private:

    static HBITMAP LoadBitmap(UINT resourceID) noexcept
    {
        HBITMAP hbitmap = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(resourceID));
        ATLASSERT(hbitmap && "Failed to load the bitmap, check resource id, etc");
        return hbitmap;
    }

    // Member variables.
    CString _text;
    Bitmap _bitmap;
};

} // end namespace MSF
