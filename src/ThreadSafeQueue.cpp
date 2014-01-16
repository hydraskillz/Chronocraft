#pragma once

#include "ThreadSafeQueue.h"

using namespace mzj;

template< class T >
ThreadSafeQueue< T >::ThreadSafeQueue()
{
	itemList.push_back( T() );
	head = itemList.begin();
	tail = itemList.end();
}

template< class T >
void ThreadSafeQueue< T >::Push( const T& item )
{
	itemList.push_back( item );
	tail = itemList.end();
	itemList.erase( itemList.begin(), head );
}

template< class T >
bool ThreadSafeQueue< T >::Pop( T& item )
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