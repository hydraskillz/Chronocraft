/*
 * Author      : Matthew Johnson
 * Date        : 29/Nov/2012
 * Description :
 *   Creates and runs a thread.
 */
#pragma once

#include <functional>

namespace mzj
{

	class Thread
	{
	public:
		Thread(){}
		Thread( std::function< void() > callee );
		~Thread();

		bool Finished() const;

	protected:
		class ThreadImplementation* pImpl;
	};

	class ThreadImplementation
	{
	public:
		virtual void runInsideThread() = 0;
		virtual void onThreadFinish() = 0;
		virtual bool isDone() const = 0;
	};

}