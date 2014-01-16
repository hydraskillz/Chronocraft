/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Base class for Blocks.
 */
#pragma once

#include "blocktypes.h"
#include <string>
#include <cassert>

namespace mzj
{
	
	class Block
	{
	public:
		Block( BlockTypeId type=AIR );
		virtual ~Block();

		// Render faces of an individual block
		void RenderFaces( const Mat4f& view, unsigned int faces );

		// Blocktype related functions
		void SetType( BlockTypeId t );
		BlockTypeId GetType() const { return type->id; }
		std::string GetTypeString() const;

		bool IsTransparent() const { assert( type ); return type->transparent; }

		// Block comparision by id
		bool operator==( const Block& other ) const
		{
			return type->id == other.type->id;
		}

		bool operator!=( const Block& other ) const
		{
			return !( *this == other );
		}

		unsigned int light;

		// Returned as out-of-bounds block
		static Block NULL_BLOCK;

		static Cube CUBE_REF;
		static const int BLOCK_SIZE = 1;

	private:
		BlockType* type;
		
	};

}