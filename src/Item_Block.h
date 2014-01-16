/*
 * Author      : Matthew Johnson
 * Date        : 19/Nov/2012
 * Description :
 *   A placable block item.
 */
#pragma once

#include "Item.h"

namespace mzj
{

	class Item_Block
		: public Item
	{
	public:
		Item_Block( const std::string& blockName, BlockTypeId type )
		{
			blockType = type;
			itemType = ITEM_BLOCK;
			name = blockName;
		}
		virtual ~Item_Block() {}

		virtual bool OnUse( Block* block, const Vec3f& pos, const Vec3f& norm );

	private:
		BlockTypeId blockType;
	};

}