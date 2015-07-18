//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

// The following macros control how and which exceptions the MSF framework 
// wil catch. C++ exceptions are not allowed to pass COM interfaces.
// By design MSF implementations should only throw _com_error or exceptions
// derived from std::exception.
// The default handler only catches these 2 exceptions. Other exceptions should
// not be thrown or converted into a com_error.
//
// A 'catch-all' variant is available. This macro will catch all C++ exceptions.
// and due to Visual C++ exception mechanism also all SEH exceptions (like GFP, etc)
// Usage of this macro could hide some programming errors, but can be usefull for 
// the release version. An unhandled exception could crash the shell, the user's main UI.
//
// It is also possible to define your own catch handler. Define MSF_COM_CATCH_HANDLER
// before including the msf header files.

#ifndef MSF_COM_CATCH_HANDLER

#ifdef MSF_COM_CATCH_ALL

#define MSF_COM_CATCH_HANDLER() \
        catch (const _com_error& e) \
        { \
            return e.Error(); \
        } \
        catch (const std::bad_alloc&) \
        { \
            return E_OUTOFMEMORY; \
        } \
        catch (const std::exception&) \
        { \
            return E_UNEXPECTED; \
        }
        catch (...) \
        { \
            ATLASSERT(!"Catched unexpected exception"); \
            return E_UNEXPECTED; \
        }

#define MSF_COM_CATCH_HANDLER_ON_ERROR(arg1, arg2) \
        catch (const _com_error& e) \
        { \
            return OnErrorHandler(e.Error(), arg1, arg2); \
        } \
        catch (const std::bad_alloc&) \
        { \
            return OnErrorHandler(E_OUTOFMEMORY, arg1, arg2); \
        }
        catch (const std::exception&) \
        { \
            return OnErrorHandler(E_UNEXPECTED, arg1, arg2); \
        }
        catch (...) \
        { \
            ATLASSERT(!"Catched unexpected exception"); \
            return OnErrorHandler(E_UNEXPECTED, arg1, arg2); \
        }

#else

#define MSF_COM_CATCH_HANDLER() \
        catch (const _com_error& e) \
        { \
            return e.Error(); \
        } \
        catch (const std::bad_alloc&) \
        { \
            return E_OUTOFMEMORY; \
        } \
        catch (const std::exception&) \
        { \
            return E_UNEXPECTED; \
        }

#define MSF_COM_CATCH_HANDLER_ON_ERROR(arg1, arg2) \
        catch (const _com_error& e) \
        { \
            return OnErrorHandler(e.Error(), arg1, arg2); \
        } \
        catch (const std::bad_alloc&) \
        { \
            return OnErrorHandler(E_OUTOFMEMORY, arg1, arg2); \
        } \
        catch (const std::exception&) \
        { \
            return OnErrorHandler(E_UNEXPECTED, arg1, arg2); \
        }

#endif // not MSF_COM_CATCH_ALL

#endif // not MSF_COM_CATCH_HANDLER
