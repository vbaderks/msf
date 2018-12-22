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
    class Bitmap final
    {
    public:
        explicit Bitmap(HBITMAP handle = nullptr) noexcept : m_handle{handle}
        {
        }

        ~Bitmap()
        {
            if (m_handle)
            {
                ATLVERIFY(DeleteObject(m_handle));
            }
        }

        Bitmap(const Bitmap&) = delete;
        Bitmap(Bitmap&&) = delete;
        Bitmap& operator=(const Bitmap&) = delete;
        Bitmap& operator=(Bitmap&&) = delete;

        HBITMAP GetHandle() const noexcept
        {
            return m_handle;
        }

    private:
        HBITMAP m_handle;
    };

    SmallBitmapHandler(std::wstring text, uint32_t resourceID) noexcept :
        m_text(std::move(text)),
        m_bitmap(LoadBitmap(resourceID))
    {
    }

    SmallBitmapHandler(uint32_t textID, uint32_t resourceID) :
        m_text(LoadResourceString(textID)),
        m_bitmap(LoadBitmap(resourceID))
    {
    }

    void InitializeItemInfo(MenuItemInfo& menuItemInfo) override
    {
        menuItemInfo.SetString(m_text);
        menuItemInfo.SetCheckMarkBitmaps(nullptr, m_bitmap.GetHandle());
    }

private:
    static HBITMAP LoadBitmap(uint32_t resourceID) noexcept
    {
        HBITMAP__* const bitmap = ::LoadBitmap(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(resourceID));
        ATLASSERT(bitmap && "Failed to load the bitmap, check resource id, etc");
        return bitmap;
    }

    // Member variables.
    std::wstring m_text;
    Bitmap m_bitmap;
};

} // end namespace msf
