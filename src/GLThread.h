#pragma once

#include "Thread.h"

//#include <functional>

namespace mzj
{

	class GLThread
		: public Thread
	{
	public:
		GLThread( std::function< void() > callee );
		~GLThread();
	};

}