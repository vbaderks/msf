//
// (C) Copyright by Victor Derks <vba64@xs4all.nl>
//
// See README.TXT for the details of the software licence.
//
#pragma once

#pragma warning(push)

#if _MSC_VER == 1400
#pragma warning(disable: 4548) // expression before comma has no effect; expected expression with side-effect
#pragma warning(disable: 4555) // expression has no effect; expected expression with side-effect
#endif

#include <conio.h>  // for _getch
#include <stdlib.h> // for EXIT_SUCCESS / EXIT_FAILURE
#include <iostream>

#pragma warning(pop)


namespace MSF
{

typedef void (*performtest_fn)();

#ifdef UNICODE
#define tcout wcout
#else
#define tcout cout
#endif

struct test_t
{
	const TCHAR*   szName;
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


	CTestRunner(const test_t* ptests, unsigned int nTests) :
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
				(void) _stscanf(argv[2], _T("%d"), &nLoop);
				for (int i = 0; i < nLoop; ++i)
				{
					PerformTest(argv[1]);
				}
			}
			break;

		default:
			std::tcout << _T("command line option unknown") << std::endl;
			return EXIT_FAILURE;
		}

		std::tcout << _T("\nAll test(s) completed, press any key to quit") << std::endl;

		if (bWaitForKeypress)
			(void) _getch();

		return EXIT_SUCCESS;
	}


private:
	void PerformTest(const test_t& test)
	{
		std::tcout << _T("Performing test: ") << test.szName;
		try
		{
			test.PerformTest();
			std::tcout << _T(" -> OK") << std::endl;
		}
		catch (...)
		{
			std::tcout << _T(" -> Failed") << std::endl;
		}
	}


	void PerformTest(const TCHAR* szName)
	{
		for (unsigned int i = 0; i < _nTests; ++i)
		{
			if (_tcscmp(_ptests[i].szName, szName) == 0)
			{
				PerformTest(_ptests[i]);
				return;
			}
		}

		std::tcout << _T("Test ") << szName << _T(" Not found") << std::endl;
	}


	void PerformAllTests()
	{
		for (unsigned int i = 0; i < _nTests; ++i)
		{
			PerformTest(_ptests[i]);
		}
	}


	const test_t*  _ptests;
	unsigned int   _nTests;
};


// cleanup defined preprocessor macros
#undef tcout


} // end MSF namespace.
