//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#include "stdafx.h"
#include "resource.h"
#include "../include/browserhelperobjectimpl.h"
#include "../include/strutil.h"
#include "../include/util.h"
#include "../include/webbrowser2ptr.h"
#include "../include/htmldocument2ptr.h"
#include "../include/htmldocument3ptr.h"
#include "../include/htmlelementcollectionptr.h"
#include "../include/htmlinputelementptr.h"
#include "../include/htmlelementptr.h"
#include <exdispid.h>

class DECLSPEC_UUID("37163A65-1E85-45AA-92F9-95E8E4D2F743") CSampleBho;

class ATL_NO_VTABLE CSampleBho :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CSampleBho, &__uuidof(CSampleBho)>,
    public CBrowserHelperObjectImpl<CSampleBho>,
    public IDispatchImpl<IDispatch>
{
public:
    BEGIN_COM_MAP(CSampleBho)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IObjectWithSite)
    END_COM_MAP()

    // In this sample attach an event handler the 'document complete' event.
    BEGIN_SINK_MAP(CSampleBho)
        SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
    END_SINK_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()


    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) MSF_NOEXCEPT
    {
        return CBrowserHelperObjectImpl<CSampleBho>::UpdateRegistry(bRegister, L"AutoSso", true);
    }


    void STDMETHODCALLTYPE OnDocumentComplete(IDispatch* pdispatch, VARIANT* /*pvarURL*/)
    {
        try
        {
            if (!IsAttachedSite(pdispatch))
                return;

            MSF::IHTMLDocument2Ptr rDoc = MSF::IWebBrowser2Ptr(pdispatch).GetDocument();
            if (rDoc == NULL)
            {
                ATLTRACE2(atlTraceCOM, 0, L"CSampleBho::OnDocumentComplete: not a HTML document.\n");
                return; // Document is not a HTML document.
            }

            // a real implementation would do something with the loaded page...
        }
        catch (const _com_error& e)
        {
            ShowMessage(CString(L"Unexpected COM exception: ") + e.ErrorMessage());
        }
        catch (const exception& e)
        {
            ShowMessage(CA2W(e.what()));
        }
        catch (...)
        {
            ShowMessage(L"Unexpected exception");
        }
    }

private:

    static void ShowMessage(PCWSTR wszText)
    {
        MessageBox(NULL, wszText, L"Browser Helper Object Sample", MB_OK);
    }
};


OBJECT_ENTRY_AUTO(__uuidof(CSampleBho), CSampleBho)
