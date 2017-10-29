//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


#include "msfbase.h"
#include "pidl.h"


namespace msf
{

template <typename TItem>
class __declspec(novtable) ExtractIcon :
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public IExtractIcon
{
public:

    class Icon
    {
    public:
        explicit Icon(HICON hicon = nullptr) :
            m_hicon(hicon)
        {
        }

        ~Icon()
        {
            dispose();
        }

        HICON operator=(HICON hicon) noexcept
        {
            dispose();
            m_hicon = hicon;
            return m_hicon;
        }

        void release() noexcept
        {
            m_hicon = nullptr;
        }

        HICON get() const noexcept
        {
            return m_hicon;
        }

    private:

        void dispose() noexcept
        {
            if (m_hicon)
            {
                ATLVERIFY(DestroyIcon(m_hicon));
                m_hicon = nullptr;
            }
        }

        HICON m_hicon;
    };

    static ATL::CComPtr<IExtractIcon> CreateInstance(const TItem& item)
    {
        ATL::CComObject<ExtractIcon<TItem> >* pinstance;
        HRESULT hr = ATL::CComObject<ExtractIcon<TItem> >::CreateInstance(&pinstance);
        if (FAILED(hr))
            RaiseException(hr);

        ATL::CComPtr<IExtractIcon> extracticon(pinstance);

        pinstance->Initialize(item);

        return extracticon;
    }

    static HICON GetIcon(HIMAGELIST himl, int i, UINT flags = 0)
    {
        HICON hicon = ImageList_GetIcon(himl, i, flags);
        RaiseExceptionIf(!hicon);
        return hicon;
    }

    DECLARE_NOT_AGGREGATABLE(ExtractIcon)

    BEGIN_COM_MAP(ExtractIcon)
        COM_INTERFACE_ENTRY(IExtractIcon)
    END_COM_MAP()

protected:
    ExtractIcon() : m_nIconIndex(-1), m_uFlags(0)
    {
    }

    void Initialize(const TItem& item)
    {
        m_pidl.CloneFrom(item.GetItemIdList());
    }

    HRESULT __stdcall GetIconLocation(UINT uFlags, LPTSTR /*szIconFile*/, UINT /*cchMax*/, _Out_ int* piIndex, _Out_ UINT* pwFlags) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractIcon::GetIconLocation, instance=%p, uFlags=%x\n", this, uFlags);

        try
        {
            m_nIconIndex = TItem(m_pidl.GetRelative()).GetIconOf(uFlags);

            *piIndex = 0; // must initialize index as it is a required out parameter.
            *pwFlags = GIL_NOTFILENAME;
            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

    HRESULT __stdcall Extract(LPCTSTR /*pszFile*/, UINT /*nIconIndex*/, _Out_opt_ HICON* phiconLarge, _Out_opt_ HICON* phiconSmall, UINT nIconSize) noexcept override
    {
        ATLTRACE2(ATL::atlTraceCOM, 0, L"ExtractIcon::Extract, instance=%p, pl=%p, ps=%p\n", this, phiconLarge, phiconSmall);

        try
        {
            if (m_nIconIndex == -1)
                return E_INVALIDARG; // GetIconLocation not called.

            HIMAGELIST himLarge;
            HIMAGELIST himSmall;
            RaiseExceptionIf(!Shell_GetImageLists(&himLarge, &himSmall));

            Icon iconLarge;
            if (phiconLarge)
            {
                if (LOWORD(nIconSize) != 32)
                    return E_INVALIDARG;

                iconLarge = GetIcon(himLarge, m_nIconIndex);
                *phiconLarge = iconLarge.get();
            }

            if (phiconSmall)
            {
                if (HIWORD(nIconSize) != 16)
                    return E_INVALIDARG;

                *phiconSmall = GetIcon(himSmall, m_nIconIndex);
            }

            iconLarge.release();

            return S_OK;
        }
        catch (...)
        {
            return ExceptionToHResult();
        }
    }

private:

    // Member variables.
    int   m_nIconIndex;
    UINT  m_uFlags;
    ItemIDList m_pidl;
};

} // namespace msf
