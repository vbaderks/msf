//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "custommenuhandler.h"


namespace msf
{

class SmallBitmapHandler : public CustomMenuHandler
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

    SmallBitmapHandler(std::wstring text, UINT resourceID) :
        m_text(text),
        m_bitmap(LoadBitmap(resourceID))
    {
    }

    SmallBitmapHandler(UINT textID, UINT resourceID) :
        m_text(LoadResourceString(textID)),
        m_bitmap(LoadBitmap(resourceID))
    {
    }

    void InitializeItemInfo(MenuItemInfo& menuItemInfo) override
    {
        menuItemInfo.SetString(m_text);
        menuItemInfo.SetCheckMarkBmps(nullptr, m_bitmap.GetHandle());
    }

private:

    static HBITMAP LoadBitmap(UINT resourceID) noexcept
    {
        HBITMAP hbitmap = ::LoadBitmap(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(resourceID));
        ATLASSERT(hbitmap && "Failed to load the bitmap, check resource id, etc");
        return hbitmap;
    }

    // Member variables.
    std::wstring m_text;
    Bitmap m_bitmap;
};

} // end namespace msf
