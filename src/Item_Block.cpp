#include "Item_Block.h"

#include "World.h"

using namespace mzj;

bool Item_Block::OnUse( Block* block, const Vec3f& pos, const Vec3f& norm )
{
	if ( block->GetType() != AIR )
	{
		World::GetInstance().GetBlock( pos+norm ).SetType( blockType );
		return true;
	}
	return false;
}