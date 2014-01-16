/*
 * Author      : Matthew Johnson
 * Date        : 19/Nov/2012
 * Description :
 *   A tool is used to do stuff to blocks.
 */
#pragma once

#include "Item.h"

namespace mzj
{
	struct ToolStats
	{
		int dirtScore;
		int stoneScore;
		int woodScore;
		int durability;
		int maxDurability;
	};

	class Item_Tool
		: public Item
	{
	public:
		Item_Tool( const std::string& toolName )
		{
			itemType = ITEM_TOOL;
			name = toolName;
		}

		virtual ~Item_Tool(){}

		virtual bool OnUse( Block* block, const Vec3f& pos, const Vec3f& norm )
		{
			if ( block->GetType() != AIR )
			{
				if ( block->GetType() == BEDROCK ) return false;

				block->SetType( AIR );
				return true;
			}
			return false;
		}

	private:
		ToolStats stats;
	};

}