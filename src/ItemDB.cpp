#include "ItemDB.h"

#include <map>

using namespace mzj;

namespace
{
	static std::map< std::string, Item* > DB;
	static bool init = false;
}

void ItemDB::InitDB()
{
	// Tools
	DB.insert( std::make_pair( std::string( "Hand" ), new Item_Tool( "Hand" ) ) );

	// Blocks
	DB.insert( std::make_pair( std::string( "Dirt" ), new Item_Block( "Dirt", DIRT ) ) );
	DB.insert( std::make_pair( std::string( "Stone" ), new Item_Block( "Stone", STONE ) ) );
	DB.insert( std::make_pair( std::string( "OBSIDION" ), new Item_Block( "OBSIDION", OBSIDION ) ) );
	DB.insert( std::make_pair( std::string( "TNT" ), new Item_Block( "TNT", TNT_FRONT ) ) );
	DB.insert( std::make_pair( std::string( "Wood" ), new Item_Block( "Wood", WOOD ) ) );
	DB.insert( std::make_pair( std::string( "Diamond" ), new Item_Block( "Diamond", DIAMOND_BLOCK ) ) );
	DB.insert( std::make_pair( std::string( "GlowStone" ), new Item_Block( "GlowStone", GLOW_STONE ) ) );

	init = true;
}

Item* ItemDB::GetItem( const std::string& name )
{
	if ( !init ) InitDB();

	auto itr = DB.find( name );

	if ( itr == DB.end() ) return DB[ "Hand" ];
	return DB[ name ];
}