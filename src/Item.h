/*
 * Author      : Matthew Johnson
 * Date        : 19/Nov/2012
 * Description :
 *   An item that can be equiped/used
 */
#pragma once

#include "Block.h"

namespace mzj
{
	class Item
	{
	public:
		Item(){}
		virtual ~Item(){}

		virtual bool OnUse( Block* block, const Vec3f& pos, const Vec3f& norm ) { return false; }

		int GetType() const { return itemType; }
		const std::string& GetName() const { return name; }

		enum
		{
			ITEM_BLOCK,
			ITEM_TOOL
		};

	protected:
		int itemType;
		std::string name;
	};
}