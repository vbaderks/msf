//
// (C) Copyright by Victor Derks
//
// See README.TXT for the details of the software licence.
//
#pragma once


namespace MSF
{

template <typename T>
class CGlobalLock
{
public:
	CGlobalLock() : m_p(NULL), m_hMem(NULL)
	{
	}


	CGlobalLock(HGLOBAL hMem) : m_p(GlobalLockThrow(hMem)), m_hMem(hMem)
	{
	}


	~CGlobalLock()
	{
		Dispose();
	}


	void Attach(HGLOBAL hMem)
	{
		void* p = GlobalLockThrow(hMem);
		
		Dispose();

		m_p = p;
		m_hMem = hMem;
	}


	void Dispose() throw()
	{
		if (m_hMem != NULL)
		{
			BOOL bResult = GlobalUnlock(m_hMem);

			#ifdef _UNICODE
				ATLASSERT(bResult || GetLastError() == NO_ERROR); (bResult);
			#else
				#ifdef _DEBUG
				DWORD dwError = GetLastError();
				if (!bResult && dwError != NO_ERROR)
				{
					if (m_hMem == m_p)
					{
						// GlobalUnlock sometimes fails for GMEM_FIXED on win9x platforms.
						// This seems to be a bug in GlobalUnlock.
						ATLTRACE2(atlTraceCOM, 0, _T("CGlobalLock::Dispose (m_p=%p, dwError=%d)\n"), m_p, dwError);
					}
					else
					{
						ATLASSERT(!"GlobalUnlock failed unexpectly");
					}
				}
				#else
					(bResult);
				#endif
			#endif

			m_hMem = NULL;
			m_p = NULL;
		}
	}


	T* get() const throw()
	{
		return reinterpret_cast<T*>(m_p);
	}

private:

	static void* GlobalLockThrow(HGLOBAL hMem)
	{
		if (hMem == NULL)
		{
			return NULL;
		}
		else
		{
			void* p = GlobalLock(hMem);
			RaiseLastErrorExceptionIf(p == NULL);
			return p;
		}
	}

	CGlobalLock& operator=(const CGlobalLock&) throw(); // not implemented by design.

	// Member variables
	void*   m_p;
	HGLOBAL m_hMem;
};

} // namespace MSF
