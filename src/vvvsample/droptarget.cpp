//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//

#include "stdafx.h"

#include "resource.h"
#include <msf.h>

using namespace MSF;

class ATL_NO_VTABLE __declspec(uuid("8EB377DD-BB66-419D-8E76-F6FDD5C020DD")) DropTarget :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<DropTarget, &__uuidof(DropTarget)>,
    public DropTargetImpl<DropTarget>
{
public:
    BEGIN_COM_MAP(DropTarget)
        COM_INTERFACE_ENTRY(IPersistFile)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) noexcept
    {
        // TODO: update
        return DropTargetImpl<DropTarget>::UpdateRegistry(
            bRegister, IDR_ICONOVERLAY, L"VVV Sample ICON Overlay ShellExtension", L"");
    }

    bool IsSupportedClipboardFormat(IDataObject* dataObject)
    {
        return CfHDrop::IsFormat(dataObject);
    }

    DWORD OnDragOver(DWORD /*grfKeyState*/, POINTL /*pt*/, DWORD /*dwEffect*/)
    {
        return 0;
    }

    DWORD OnDrop(IDataObject* dataObject, DWORD /*grfKeyState*/, POINTL /*pt*/, DWORD dwEffect)
    {
        CfHDrop cfhdrop(dataObject);

        unsigned int nFiles = cfhdrop.GetFileCount();
        for (unsigned int i = 0; i < nFiles; ++i)
        {
            //AddItem(cfhdrop.GetFile(i));
        }

        return dwEffect;
    }
};

OBJECT_ENTRY_AUTO(__uuidof(DropTarget), DropTarget)
