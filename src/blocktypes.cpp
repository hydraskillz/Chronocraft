#include "blocktypes.h"
#include "TextureFactory.h"
#include "ResourceManager.h"
#include "gl.h"
#include <cassert>

using namespace mzj;

namespace
{
	static BlockType* BlockTypeDB[ NUM_TYPES ];
}

BlockType* const BlockTypes::GetBlockType( BlockTypeId id )
{
	// First time initialization
	if ( *BlockTypeDB == 0 )
		for ( BlockTypeId i = AIR; i < NUM_TYPES; ++i )
			BlockTypeDB[ i ] = new BlockType( BuildTextureVBOId( i ), i, i == AIR );

	assert( id >= 0 && id < NUM_TYPES );

	return BlockTypeDB[ id ];
}

Vec2f* BlockTypes::BuildTextureVBOId( BlockTypeId id )
{
	if ( id == AIR ) return 0;
	Vec2f* texCoords = new Vec2f[36];
	if ( id == GRASS )
	{
		id =  DIRT_GRASS;
		BuildFace( Front,  id, texCoords );
		BuildFace( Back,   id, texCoords );
		BuildFace( Top,    GRASS, texCoords );
		BuildFace( Bottom, DIRT, texCoords );
		BuildFace( Left,   id, texCoords );
		BuildFace( Right,  id, texCoords );
	}
	else if ( id == TNT_FRONT )
	{
		BuildFace( Front,  TNT_FRONT, texCoords );
		BuildFace( Back,   TNT_FRONT, texCoords );
		BuildFace( Top,    TNT_TOP, texCoords );
		BuildFace( Bottom, TNT_BOTTOM, texCoords );
		BuildFace( Left,   TNT_FRONT, texCoords );
		BuildFace( Right,  TNT_FRONT, texCoords );
	}
	else if ( id == TREE_SIDE )
	{
		BuildFace( Front,  TREE_SIDE, texCoords );
		BuildFace( Back,   TREE_SIDE, texCoords );
		BuildFace( Top,    TREE_TOP, texCoords );
		BuildFace( Bottom, TREE_TOP, texCoords );
		BuildFace( Left,   TREE_SIDE, texCoords );
		BuildFace( Right,  TREE_SIDE, texCoords );
	}
	else
	{
		BuildFace( Front,  id, texCoords );
		BuildFace( Back,   id, texCoords );
		BuildFace( Top,    id, texCoords );
		BuildFace( Bottom, id, texCoords );
		BuildFace( Left,   id, texCoords );
		BuildFace( Right,  id, texCoords );
	}

	/*
	unsigned int TVBO = 0;
	// Generate And Bind The Texture Coordinate Buffer
	glGenBuffersARB( 1, &TVBO );						// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, TVBO );		// Bind The Buffer
	// Load The Data
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 36*2*sizeof(float), texCoords, GL_STATIC_DRAW_ARB );
	delete[] texCoords;
	return TVBO;
	*/
	return texCoords;
}

void BlockTypes::BuildFace( unsigned int face, unsigned int texId, Vec2f* uv )
{
	const float delta = 16.0f / 256.0f;		// Block texture width and height
	const float x = ((texId-1)%16)*delta;	// Texture coords
	const float y = ((texId-1)/16)*delta;
	const float w = x + delta;
	const float h = y + delta;

	if ( face == Front || face == Left )
	{
		face = (face == Front ? 0 : 3)*6;

		uv[face  ].Set( w, y );
		uv[face+1].Set( x, y );
		uv[face+2].Set( x, h );
		uv[face+3].Set( x, h );
		uv[face+4].Set( w, h );
		uv[face+5].Set( w, y );
	}
	else if ( face == Back || face == Bottom )
	{
		face = (face == Back ? 5 : 4)*6;

		uv[face  ].Set( w, h );
		uv[face+1].Set( x, h );
		uv[face+2].Set( x, y );
		uv[face+3].Set( x, y );
		uv[face+4].Set( w, y );
		uv[face+5].Set( w, h );
	}
	else if ( face == Right || face == Top )
	{
		face = (face == Right ? 1 : 2)*6;

		uv[face  ].Set( x, y );
		uv[face+1].Set( x, h );
		uv[face+2].Set( w, h );
		uv[face+3].Set( w, h );
		uv[face+4].Set( w, y );
		uv[face+5].Set( x, y );
	}
}