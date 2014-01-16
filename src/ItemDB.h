/*
 * Author      : Matthew Johnson
 * Date        : 19/Nov/2012
 * Description :
 *   A database to hold all the Items.
 */
#pragma once

#include "Items.h"

#include <string>

namespace mzj
{
	namespace ItemDB
	{
		void InitDB();
		Item* GetItem( const std::string& name );
	}
}