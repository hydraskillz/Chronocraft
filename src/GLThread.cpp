#include "GLThread.h"

//#include <Windows.h>
#include "WinThread.h"
#include <cassert>

#include <glut.h>

namespace mzj
{
	//DWORD WINAPI threadWrapper( LPVOID pParameter );
	class GLThreadImplementation
		: public ThreadImplementation
	{
	public:
		GLThreadImplementation( std::function< void() > callee )
			: myCallee( callee )
		{
			// Setup OpenGL for multithreading

			// 1) Create a context for this thread
			hDC = wglGetCurrentDC();
			hRC = wglCreateContext( hDC );

			// 2) Tell the contexts to share OpenGL
			wglShareLists( wglGetCurrentContext(), hRC );

			hThread = ::CreateThread( NULL, 0, threadWrapper,
				this,
				0,
				0 );
		}

		~GLThreadImplementation()
		{
			onThreadFinish();
			::TerminateThread( hThread, 0 );
			::CloseHandle( hThread );
		}

	private:
		HANDLE hThread;
		HDC hDC;
		HGLRC hRC;

		std::function< void() > myCallee;

		void runInsideThread()
		{
			assert( myCallee );
			assert( hDC );
			assert( hRC );

			wglMakeCurrent( hDC, hRC );

			myCallee();

			onThreadFinish();
		}

		void onThreadFinish()
		{
			if( hRC )
			{
				wglMakeCurrent( NULL, NULL );
				wglDeleteContext( hRC );
				hRC = NULL;
			}
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
	/*
	DWORD WINAPI threadWrapper( LPVOID pParameter )
	{
		mzj::GLThreadImplementation* pThreadImplementation = 
			reinterpret_cast< mzj::GLThreadImplementation* >( pParameter );

		assert( pParameter );

		pThreadImplementation->runInsideThread();

		return 0;
	}
	*/
}

////////////////////////////////////////////////////////////

namespace mzj
{
	GLThread::GLThread( std::function< void() > callee )
	{
		pImpl = new GLThreadImplementation( callee );
	}

	GLThread::~GLThread()
	{
		delete pImpl;
	}
}