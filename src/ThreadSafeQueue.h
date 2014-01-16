#pragma once

#include <list>

namespace mzj
{
	
	template< class T >
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue()
		{
			itemList.push_back( T() );
			head = itemList.begin();
			tail = itemList.end();
		}

		void Push( const T& item )
		{
			itemList.push_back( item );
			tail = itemList.end();
			itemList.erase( itemList.begin(), head );
		}

		bool Pop( T& item )
		{
			typename QList::iterator next = head;

			++next;

			if ( next != tail )
			{
				head = next;
				item = *head;
				return true;
			}
			return false;
		}

	private:
		typedef std::list< T > QList;
		QList itemList;
		typename QList::iterator head, tail;
	};

}