/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Defines for the different block types
 */

#include "math_util.h"
#include "Cube.h"

namespace mzj
{
	// Unique identifier for each BlockType
	typedef unsigned int BlockTypeId;

	struct BlockType
	{
		BlockType( Vec2f* textureCoords, BlockTypeId id, bool trans )
			: texCoords( textureCoords )
			, id( id )
			, transparent( trans )
		{}
		~BlockType()
		{}
		const BlockTypeId id;		// Id/Lookup index
		const Vec2f* texCoords;		// Texture offsets
		const bool transparent;		// Render through or not
	};

	namespace BlockTypes
	{
		// Public
		BlockType* const GetBlockType( BlockTypeId id );

		// Private
		static Vec2f* BuildTextureVBOId( BlockTypeId id );
		static void BuildFace( unsigned int face, unsigned int texId, Vec2f* uv );
	}

	// Block types ordered row manjor from bitmap
	enum blocktypeids
	{
		AIR	= 0,
		GRASS,
		STONE,
		DIRT,
		DIRT_GRASS,
		WOOD,
		STONE_STEP,
		PRESSURE_PLATE_STONE,
		BRICK,
		TNT_FRONT,
		TNT_TOP,
		TNT_BOTTOM,
		WEB,
		FLOWER_RED,
		FLOWER_YELLOW,
		WATER,
		SAPLING,
		COBBLE_STONE,
		BEDROCK,
		SAND,
		GRAVEL,
		TREE_SIDE,
		TREE_TOP,
		UNUSED_1,
		GOLD_BLOCK,
		DIAMOND_BLOCK,
		UNUSED_2,
		UNUSED_3,
		UNUSED_4,
		MUSHROOM_RED,
		MUSHROOM_BROWN,
		SAPLING_PINE,
		UNUSED_5,
		GOLD,
		IRON,
		COAL,
		BOOKS,
		COBBLESTONE_VINES,
		OBSIDION,
		UNUSED_6,
		UNUSED_7,
		UNUSED_8,
		UNUSED_9,
		UNUSED_10,
		CRAFTING_TABLE_TOP,
		OVEN_FRONT_OFF,
		OVEN_SIDE,
		UNUSED_11,
		UNUSED_12,
		UNUSED_13,
		GLASS,
		DIAMOND,
		REDSTONE,
		LEAVES_01,
		UNUSED_15,
		UNUSED_16,
		UNUSED_17,
		UNUSED_18,
		UNUSED_19,
		UNUSED_20,
		CRAFTING_TABLE_FRONT,
		CRAFTING_TABLE_SIDE,
		OVEN_FRONT_ON,
		OVEN_TOP,
		UNUSED_21,
		WOOL,
		CAGE,
		SNOW,
		ICE,
		SNOW_SIDE,
		CACTUS,
		CACTUS_SIDE,
		CACTUS_TOP,
		UNUSED_22,
		SUGAR_CANE,
		MUSIC_BOX,
		MUSIC_BOX_TOP,
		LILLY_PAD,
		UNUSED_23,
		UNUSED_24,
		SAPLING_WHITE,
		TORCH,
		DOOR_WOOD_TOP,
		DOOR_STEEL_TOP,
		LADDER,
		HATCH,
		BARS,
		UNUSED_25,
		UNUSED_26,
		WHEAT_1,
		WHEAT_2,
		WHEAT_3,
		WHEAT_4,
		WHEAT_5,
		WHEAT_6,
		WHEAT_7,
		WHEAT_8,
		LEVER,
		DOOR_WOOD_BOTTOM,
		DOOR_STEEL_BOTTOM,
		TORCH_REDSTONE,
		STONE_MOSS,
		SONE_CRACKED,
		PUMPKIN_TOP,
		UNUSED_27,
		UNUSED_28,
		GLOW_STONE,
		NUM_TYPES
	};
}