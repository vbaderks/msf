//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#include "custom_menu_handler.h"

namespace msf {

class SmallBitmapHandler final : public CustomMenuHandler
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

        [[nodiscard]] HBITMAP GetHandle() const noexcept
        {
            return m_handle;
        }

    private:
        HBITMAP m_handle;
    };

    SmallBitmapHandler(std::wstring text, uint32_t resourceId) noexcept :
        m_text(std::move(text)),
        m_bitmap(LoadBitmap(resourceId))
    {
    }

    SmallBitmapHandler(uint32_t textId, uint32_t resourceId) :
        m_text(LoadResourceString(textId)),
        m_bitmap(LoadBitmap(resourceId))
    {
    }

    void InitializeItemInfo(MenuItemInfo& menuItemInfo) override
    {
        menuItemInfo.SetString(m_text);
        menuItemInfo.SetCheckMarkBitmaps(nullptr, m_bitmap.GetHandle());
    }

private:
    [[nodiscard]] static HBITMAP LoadBitmap(uint32_t resourceId) noexcept
    {
        HBITMAP__* const bitmap = ::LoadBitmap(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(resourceId));
        ATLASSERT(bitmap && "Failed to load the bitmap, check resource id, etc");
        return bitmap;
    }

    // Member variables.
    std::wstring m_text;
    Bitmap m_bitmap;
};

} // end namespace msf
