/*
 * Author      : Matthew Johnson
 * Date        : 18/Sep/2012
 * Description :
 *   Base class for Singleton objects.
 */
#pragma once

// Macro for initializing Singleton in constructor and disabling the warning
#define INIT_SINGLETON( T ) \
__pragma( warning(push) ) \
__pragma( warning(disable: 4355) ) \
Singleton< T >( this ) \
__pragma( warning(pop) )

namespace mzj
{

	template< class T >
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			if ( !DoesExist() )
				s_pInstance = new T();
			return *s_pInstance;
		}
		
		static bool DoesExist()
		{
			return s_pInstance != nullptr;
		}

		static void Delete()
		{
			if ( s_pInstance != nullptr)
				delete s_pInstance;
		}

	protected:
		Singleton( T* pInstance )
			//: s_pInstance( pInstance )
		{
			s_pInstance = pInstance;
		}

		virtual ~Singleton()
		{}

	private:
		static T* s_pInstance;
	};

	template< class T >
	T* Singleton< T >::s_pInstance;

}