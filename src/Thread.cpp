#include "Thread.h"

//#include <Windows.h>
#include "WinThread.h"
#include <cassert>

namespace mzj
{
//	DWORD WINAPI threadWrapper( LPVOID pParameter );
	class WinThreadImplementation
		: public ThreadImplementation
	{
	public:
		WinThreadImplementation( std::function< void() > callee )
			: myCallee( callee )
		{
			hThread = ::CreateThread( NULL, 0, threadWrapper,
				this,
				0,
				0 );
		}

		~WinThreadImplementation()
		{
			onThreadFinish();
			::TerminateThread( hThread, 0 );
			::CloseHandle( hThread );
		}

	private:
		HANDLE hThread;
		HDC hDC;

		std::function< void() > myCallee;

		void runInsideThread()
		{
			assert( myCallee );
			assert( hDC );

			myCallee();

			onThreadFinish();
		}

		void onThreadFinish()
		{
			//::TerminateThread( hThread, 0 );
			//::CloseHandle( hThread );
		}

		bool isDone() const
		{
			DWORD res = ::WaitForSingleObject( hThread, 0 );
			if ( res == WAIT_OBJECT_0 ) return true;
			return false;
		}

		friend DWORD WINAPI threadWrapper( LPVOID pParameter );
	};

////////////////////////////////////////////////////////////

	DWORD WINAPI threadWrapper( LPVOID pParameter )
	{
		mzj::ThreadImplementation* pThreadImplementation = 
			reinterpret_cast< mzj::ThreadImplementation* >( pParameter );

		assert( pParameter );

		pThreadImplementation->runInsideThread();

		return 0;
	}
}

////////////////////////////////////////////////////////////

namespace mzj
{
	Thread::Thread( std::function< void() > callee )
		: pImpl( new WinThreadImplementation( callee ) )
	{
	}

	bool Thread::Finished() const
	{
		return pImpl->isDone();
	}

	Thread::~Thread()
	{
		delete pImpl;
	}
}