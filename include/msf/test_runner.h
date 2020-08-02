//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once

#pragma warning(push)

#include <conio.h>  // for _getch
#include <cstdlib> // for EXIT_SUCCESS / EXIT_FAILURE
#include <iostream>
#include <tchar.h>

#pragma warning(pop)


namespace msf
{

typedef void (*performtest_fn)();

#ifdef UNICODE
#define tcout wcout
#else
#define tcout cout
#endif

struct test_t
{
    const wchar_t*   szName;
    performtest_fn PerformTest;
};


class CTestRunner
{
public:
    static int Main(const test_t* ptests, unsigned int nTests, int argc, _TCHAR* argv[])
    {
        CTestRunner testrunner(ptests, nTests);

        return testrunner.Main(argc, argv);
    }


    CTestRunner(const test_t* ptests, unsigned int nTests) noexcept :
        _ptests(ptests), _nTests(nTests)
    {
    }


    int Main(int argc, _TCHAR* argv[])
    {
        bool bWaitForKeypress = true;

        switch (argc)
        {
        case 1:
            PerformAllTests();
            break;

        case 2:
            PerformTest(argv[1]);
            break;

        case 3:
            {
                bWaitForKeypress = false;
                int nLoop = 0;
                static_cast<void>(_stscanf(argv[2], L"%d", &nLoop));
                for (int i = 0; i < nLoop; ++i)
                {
                    PerformTest(argv[1]);
                }
            }
            break;

        default:
            std::tcout << L"command line option unknown" << std::endl;
            return EXIT_FAILURE;
        }

        std::tcout << L"\nAll test(s) completed, press any key to quit" << std::endl;

        if (bWaitForKeypress)
        {
            static_cast<void>(_getch());
        }

        return EXIT_SUCCESS;
    }


private:
    static void PerformTest(const test_t& test)
    {
        std::tcout << L"Performing test: " << test.szName;
        try
        {
            test.PerformTest();
            std::tcout << L" -> OK" << std::endl;
        }
        catch (...)
        {
            std::tcout << L" -> Failed" << std::endl;
        }
    }


    void PerformTest(const wchar_t* szName) const
    {
        for (unsigned int i = 0; i < _nTests; ++i)
        {
            if (_tcscmp(_ptests[i].szName, szName) == 0)
            {
                PerformTest(_ptests[i]);
                return;
            }
        }

        std::tcout << L"Test " << szName << L" Not found" << std::endl;
    }


    void PerformAllTests() const
    {
        for (unsigned int i = 0; i < _nTests; ++i)
        {
            PerformTest(_ptests[i]);
        }
    }


    const test_t* _ptests;
    unsigned int _nTests;
};


// cleanup defined preprocessor macros
#undef tcout


} // end msf namespace.
