#include "Block.h"
#include "TextureFactory.h"
#include "Renderer.h"

using namespace mzj;

Cube Block::CUBE_REF( (float)BLOCK_SIZE );

Block Block::NULL_BLOCK;

Block::Block( BlockTypeId typeId )
	: type( BlockTypes::GetBlockType( typeId ) )
	, light( 0 )
{
}

Block::~Block()
{
	type = nullptr;
}

void Block::SetType( BlockTypeId tid )
{
	type = BlockTypes::GetBlockType( tid );
}

std::string Block::GetTypeString() const
{
	switch ( type->id )
	{
	case AIR:				return "Air";
	case DIRT:				return "Dirt";
	case DIRT_GRASS:		return "DirtGrass";
	case GRASS:				return "Grass";
	case STONE:				return "Stone";
	case BEDROCK:			return "Bedrock";
	case TREE_SIDE:
	case TREE_TOP:			return "Tree";
	case LEAVES_01:			return "Leaves";
	default:				return "Unknown";
	}
}

void Block::RenderFaces( const Mat4f& view, unsigned int faces )
{
	if ( faces )
	{
		Renderer& renderer = Renderer::GetInstance();
		renderer.ApplyTransformation( view );
		renderer.RenderCubeFaces( faces, type->texCoords );
	}
}