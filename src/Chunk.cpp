#include "Chunk.h"
#include "PerlinNoise.h"
#include "World.h"
#include "gl.h"

//#include "Renderer.h"

#include "ResourceManager.h"
#include "TextureFactory.h"

using namespace mzj;

const float Chunk::TEXTURE_DELTA = 1.0f / 16.0f;

Chunk Chunk::NULL_CHUNK;

Chunk::Chunk()
	: idVBO( 0 )
	, numVerts( 0 )
	, needsRebuild( true )
	, generationDone( false )
	, texture( nullptr )
	, lock( false )
	, renderLock( false )
	, lightLock( false )
	, genDetailDone( false )
{
//	Block::NULL_BLOCK.SetType( AIR );
	//texture = TextureFactory::CreateOrGetTexture( ResourceManager::GetResourcePath( RES_TEXTURE ) + "terrain.png" );
	NULL_CHUNK.generationDone = true;
}

Chunk::~Chunk()
{
	DestroyRenderData();
}

bool Chunk::Generate( int dx, int dz )
{
	if ( generationDone ) return false;
	//needsRebuild = true;
	cx = dx;
	cz = dz;

//	texture = TextureFactory::CreateOrGetTexture( ResourceManager::GetResourcePath( RES_TEXTURE ) + "terrain.png" );

	ForEachBlock( [&] ( Block& block, int x, int y, int z )
	{
		block.SetType( GenerateBlockType( x + dx * X_SIZE, y, z + dz * Z_SIZE ) );
	});

	// Place grass
	for ( int x = 0; x < X_SIZE; ++ x )
	for ( int z = 0; z < Z_SIZE; ++ z )
	for ( int y = 0; y < Y_SIZE; ++y )
	{
		if ( blocks[x][y][z].GetType() == DIRT && y + 1 < Y_SIZE && blocks[x][y+1][z].GetType() == AIR )
			blocks[x][y][z].SetType( GRASS );
	}

	// Bedrock
	GenBedrock();

	generationDone = true;
	return true;
}

bool Chunk::GenFeatures( int dx, int dz )
{
	if ( genDetailDone ) return false;
	// Place trees
	Vec2i lastPlant;
	for ( int x = 0; x < X_SIZE; ++ x )
	for ( int z = 0; z < Z_SIZE; ++ z )
	for ( int y = 0; y < Y_SIZE; ++y )
	{
		if ( blocks[x][y][z].GetType() == GRASS && y + 1 < Y_SIZE && blocks[x][y+1][z].GetType() == AIR )
		{
			Vec2i pos( x, z );
			double v[] = { x/(double)X_SIZE, z/(double)Z_SIZE };
			double n = noise2( v );
			if ( (lastPlant - pos).Length() > 8 && n > 0.09 )
			{
				GenTree( x + dx * X_SIZE, y, z + dz * Z_SIZE );
				lastPlant = pos;
			}
		}
	}

	needsRebuild = true;
	genDetailDone = true;
	return true;
}

void GetNeighborsAir( const Vec3i& pos, std::vector< Block* >& blockVec )
{
	World& world = World::GetInstance();
	Block* blocks[6];
	blocks[0] = &world.GetBlock( pos.x+1, pos.y  , pos.z   );
	blocks[1] = &world.GetBlock( pos.x-1, pos.y  , pos.z   );
	blocks[2] = &world.GetBlock( pos.x  , pos.y  , pos.z+1 );
	blocks[3] = &world.GetBlock( pos.x  , pos.y  , pos.z-1 );
	blocks[4] = &world.GetBlock( pos.x  , pos.y+1, pos.z   );
	blocks[5] = &world.GetBlock( pos.x  , pos.y-1, pos.z   );

	for ( int i = 0; i < 6; ++i )
	{
		if ( blocks[i]->GetType() == AIR ) blockVec.push_back( blocks[i] );
	}
}

inline Vec3i& Chunk::LocalToWorld( Vec3i& local ) const
{
	local.Set( X_SIZE * cx + local.x, local.y, Z_SIZE * cz + local.z );
	return local;
}

inline unsigned int diminishLight( unsigned int light )
{
	if ( light == 0 ) return 0;
	if ( light >= Chunk::LIGHT_MAX ) return Chunk::LIGHT_MAX - 1;
	return light - 1;
}

inline unsigned int undiminishLight( unsigned int light )
{
	if ( light == 0 ) return 0;
	if ( light == Chunk::LIGHT_MAX ) return light;
	return light + 1;
}

// World ambient lighting
// This is local to the chunk
bool Chunk::GenLighting()
{
	if ( !generationDone ) return false;

//	while ( lightLock );
//	lightLock = true;

	Print( "Generating ambient lights for " << cx << ", " << cz );

//	while ( lock ) ;

	for ( int x = 0; x < X_SIZE; ++ x )
	for ( int z = 0; z < Z_SIZE; ++ z )
	{
		unsigned int currentLighting = 4;//LIGHT_SKY;
		for ( int y = Y_SIZE-1; y >= 0; --y )
		{
			if ( !blocks[x][y][z].IsTransparent() )
			{
				//break;

				// Diminish light as it passes through blocks
				currentLighting--;
				if ( currentLighting < 0 ) currentLighting = 0;
			}
			blocks[x][y][z].light = currentLighting;
		}
	}
	
//	lightLock = false;
	Print( "... done\n" );
	return true;
}

// Light spreading from lightsources
// This is a global algorithm and can modify other chunks
bool Chunk::GenLightingSpread()
{
	Print( "Generating source lights for " << cx << ", " << cz );
	World& world = World::GetInstance();

	for ( int x = 0; x < X_SIZE; ++ x )
	for ( int z = 0; z < Z_SIZE; ++ z )
	{
		for ( int y = Y_SIZE-1; y >= 0; --y )
		{
			// Is the block a light source?
			if ( world.GetBlock( X_SIZE * cx + x, y, Z_SIZE * cz + z ).GetType() == GLOW_STONE )
			{
				// 6 directions light can spread
				const Vec3i dirs[6] =
				{
					Vec3i( 0, 0, 1 ),
					Vec3i( 0, 1, 0 ),
					Vec3i( 1, 0, 0 ),
					Vec3i( 0, 0, -1 ),
					Vec3i( 0, -1, 0 ),
					Vec3i( -1, 0, 0 ),
				};

				// List of light sources
				std::vector< Vec3i > sources;

				// Kick off with the glowstone block
				sources.push_back( Vec3i( X_SIZE * cx + x, y, Z_SIZE * cz + z ) );
				world.GetBlock( sources[0] ).light = 12U;

				do
				{
					Vec3i pos = sources.back();
					sources.pop_back();
					unsigned int oldLight = world.GetBlock( pos ).light;
					unsigned int newLight = diminishLight( oldLight );

					for ( int i = 0; i < 6; ++i )
					{
						Vec3i newPos = pos + dirs[i];

						if ( world.GetBlock( newPos ).GetType() == AIR )
						{
							/* Bugs out if near edge of world so commenting it out for now.
							// Brighter than us, add as source
							if ( world.GetBlock( newPos ).light > undiminishLight( oldLight ) )
							{
								sources.push_back( newPos );
							}
							*/
							// Dimmer than us, light us and add it as a source
							if ( world.GetBlock( newPos ).light < newLight )
							{
								world.GetBlock( newPos ).light = newLight;
								sources.push_back( newPos );
							}
						}
					}
				} while ( sources.size() != 0 );
			}
	
		}
	}

	// Chunk will need its VBO rebuilt
	needsRebuild = true;

	Print( "... done\n" );
	return true;
}

// TODO better terrain generation
BlockTypeId Chunk::GenerateBlockType( int x, int y, int z )
{
	
	BlockTypeId id = AIR;

	double px = x / (double)X_SIZE;
	double py = y / (double)Y_SIZE;
	double pz = z / (double)Z_SIZE;

	double TerrainFrequency = 2.0;

	double n = noise3D( 1, px, py, pz );
	n = (n + 1.0) / 2.0;
	double max_height = 0.9;
	double density = n * ( 1.0 - py / max_height );

	
	/*
	if ( density < -0.1 )
	{
		// Caves
		double pFreq = 6;
		double caveX = px + noise3D( 3, px*pFreq, py*pFreq, pz*pFreq )*0.25;
		double caveY = py + noise3D( 3, px*pFreq, py*pFreq, pz*pFreq )*0.25;
		double caveZ = pz + noise3D( 3, px*pFreq, py*pFreq, pz*pFreq )*0.25;
		
		double caveFreq = 2.0;
		int cave = 1.0 - abs( noise3D( 1,caveX*caveFreq, caveY*caveFreq, caveZ*caveFreq ) ) > 0.95;
		cave    *= 1.0 - abs( noise3D( 1,caveX*caveFreq, caveY*caveFreq, caveZ*caveFreq ) ) > 0.95;
		
		if ( cave )
		{
			id = AIR;
		}
		else
		{
			id = STONE;
		}
	}*/
	if ( density > 0.1 )
	{
		if ( py < 0.65 )
		{
			if ( density > 0.12 )
				id = STONE;
			else
				id = GRAVEL;
		}
		else if ( py < 0.7 )
			id = SAND;
		else
			id = DIRT;
	}
	else
	{
		if ( py < 0.7 )
			id = WATER;
	}

	return id;
}

void Chunk::GenBedrock()
{
	for ( int x = 0; x < X_SIZE; ++ x )
	for ( int y = 0; y < 2; ++ y )
	for ( int z = 0; z < Z_SIZE; ++ z )
	{
		blocks[x][y][z].SetType( BEDROCK );
	}
}

void Chunk::GenTree( int x, int y, int z )
{
	World& world = World::GetInstance();

	world.GetBlock( x, y, z ).SetType( DIRT );

	int height = randInRange( 3, 4 );

	for ( int h = 1; h <= height; ++h )
	{
		world.GetBlock( x, y+h, z ).SetType( TREE_SIDE );
	}
	
	for ( int rx = -1; rx < 2; ++rx )
	{
		for ( int rz = -1; rz < 2; ++ rz )
		{
			if ( rx == 0 && rz == 0 ) continue;

			Block& block = world.GetBlock( x + rx, y + height, z + rz );
			if ( block.GetType() == AIR )
			{
				block.SetType( LEAVES_01 );
			}
		}
	}

	world.GetBlock( x, y + height + 1, z ).SetType( LEAVES_01 );
}

int Chunk::Render( const Camera& view, int cx, int cz )
{
	if ( idVBO )
	{
		// Can't render when locked
		if ( renderLock ) return 0;
		// Lock renderer
		renderLock = true;

		if ( texture == nullptr )
			texture = TextureFactory::CreateOrGetTexture( ResourceManager::GetResourcePath( RES_TEXTURE ) + "terrain.png" );

		Mat4f trans;
		trans.Translate( Vec3f( (float) (cx * X_SIZE * Block::BLOCK_SIZE), 0.0f, (float) (cz * Z_SIZE * Block::BLOCK_SIZE) ) );

		Mat4f vMat = (trans * view.GetTransform()); 
		glLoadMatrixf( vMat.GetArray() );

		glColor3f( 1, 1, 1 );
		TextureFactory::ApplyTexture( texture );

		glBindBuffer( GL_ARRAY_BUFFER, idVBO );

		// TODO move to member constant
	#define NUM_FLOATS_PER_VERTEX 11

		const unsigned int stride = sizeof( float ) * NUM_FLOATS_PER_VERTEX;

		// Setup vertex data structure
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, stride, 0 );

		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, stride, reinterpret_cast< void* >( sizeof( float ) * 3 ) );

		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( 3, GL_FLOAT, stride, reinterpret_cast< void* >( sizeof( float ) * ( 3 + 2 ) ) );

		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer( GL_FLOAT, stride, reinterpret_cast< void* >( sizeof( float ) * ( 3 + 2 + 3 ) ) );

		// Draw
		glDrawArrays( GL_TRIANGLES, 0, numVerts );

		// Disable vertex data
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
	}
	// Unlock render
	renderLock = false;
	return numVerts / 3;
}

const Block& Chunk::GetBlock( int x, int y, int z ) const
{
	if ( x < 0 || x >= X_SIZE ||
		 y < 0 || y >= Y_SIZE ||
		 z < 0 || z >= Z_SIZE )
		 return Block::NULL_BLOCK;
	return blocks[x][y][z];
}

Block& Chunk::GetBlock( int x, int y, int z )
{
	if ( x < 0 || x >= X_SIZE ||
		 y < 0 || y >= Y_SIZE ||
		 z < 0 || z >= Z_SIZE )
		 return Block::NULL_BLOCK;
	return blocks[x][y][z];
}

bool Chunk::SetBlockType( float x, float y, float z, BlockTypeId id )
{
	int _x = (int)abs(x);
	int _y = (int)abs(y);
	int _z = (int)abs(z);

	if ( _x < 0 || _x > X_SIZE || _y < 0 || _y > Y_SIZE || _z < 0 || _z > Z_SIZE )
		return false;

	if ( blocks[_x][_y][_z].GetType() == id ) return false;

	blocks[_x][_y][_z].SetType( id );

	return true;
}



void Chunk::EmitCubeFaces( std::vector< float >& inOutVertexElem, unsigned int faces, BlockTypeId id, const Vec3f& translation, const Vec3f color[6]  )
{
	for ( int f = 0; faces; faces >>= 1, f += 6 )
	{
		if ( faces & 1 )
		{
			// Output face
			for ( int i = 0; i < 6; ++i )
			{
				// Output verts
				const Vec3f& vert = Block::CUBE_REF.GetCubeVertex( f + i );
				for ( int v = 0; v < 3; ++v )
				{
					inOutVertexElem.push_back( vert[ v ] + translation[ v ] );
				}

				// Output tex coords
				BlockType* const bt = BlockTypes::GetBlockType( id );
				inOutVertexElem.push_back( bt->texCoords[ f + i ][0] );
				inOutVertexElem.push_back( bt->texCoords[ f + i ][1] );
				/*
				const Vec2f& texVert = Block::CUBE_REF.GetCubeTexVert( f + i );
				inOutVertexElem.push_back( ( texVert[0] + ((id-1)%16) ) * TEXTURE_DELTA );
				inOutVertexElem.push_back( ( texVert[1] + ((id-1)/16) ) * TEXTURE_DELTA );
				*/

				// Output colors
				for ( int c = 0; c < 3; ++c )
				{
					//assert( f/6 >=6 );
					int k = f/6;
					if ( k >= 6 ) k = 5;
					inOutVertexElem.push_back( color[k][ c ] );
				}

				// Output normals
				const Vec3f& normal = Block::CUBE_REF.GetCubeNormal( f + i );
				for ( int n = 0; n < 3; ++n )
				{
					inOutVertexElem.push_back( normal[ n ] );
				}
			}
		}
	}
}

bool Chunk::GenerateRenderData()
{
	if ( !generationDone ) return false;
	if ( !needsRebuild ) return false;

	while ( lock == true ) ;
		lock = true;

	needsRebuild = false;

	Print( "Generating VBOs for chunk at " << cx << ", " << cz << "... " );
	//DestroyRenderData();

	World& world = World::GetInstance();

	// Create vertex data
	std::vector< float > vertexElements;

	ForEachBlock( [&] ( Block& b, int x, int y, int z )
	{
		// Skip invisible blocks
		if ( !b.IsTransparent() )
		{
			// Translate to where block needs to be drawn
			const float halfsize = Block::BLOCK_SIZE / 2.0f;
			Vec3f translation(
				Block::BLOCK_SIZE * ( x + halfsize ),
				Block::BLOCK_SIZE * ( y + halfsize ),
				Block::BLOCK_SIZE * ( z + halfsize ));

			unsigned int facemask = 0;
			Vec3f faceColors[6];

			static const int faces[] = { Left, Back, Bottom, Top, Front, Right };
			static const int colorFaces[] = { 3, 5, 4, 2, 0, 1 };
			/*
			for ( int face = 0; face < 6; ++face )
			{
				std::vector< const Block* > neighbors;
				GetNeighbors( Vec3i( cx * X_SIZE + x, y, cz * Z_SIZE + z ), neighbors );

				for ( auto itr = neighbors.front(); itr != neighbors.back(); ++itr )
				{
					if ( itr->IsTransparent() )
					{
						facemask |= faces[ face ];

						const float brightness = lerp( 0.1f, 1.0f, itr->light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ colorFaces[ face ] ].Set( brightness, brightness, brightness );
					}
				}
			}
			*/

			/*
			Order:
			left
			back
			bottom
			top
			front
			right
			*/
			
			
			int face = 0;
			for ( int ix = -1; ix <= 1; ++ix )
			{
				for ( int iz = -1; iz <= 1; ++iz )
				{
					if ( ix != 0 && iz != 0 ) continue;

					if ( ix == 0 && iz == 0 )
					{
						for ( int iy = -1; iy <= 1; ++iy )
						{
							if ( iy == 0 ) continue;
							Block& block = world.GetBlock( cx * X_SIZE + x + ix, y + iy, cz * Z_SIZE + z + iz );
							if ( block.IsTransparent() )
							{
								facemask |= faces[ face ];

								const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
								faceColors[ colorFaces[ face ] ].Set( brightness, brightness, brightness );
							}
							face++;
						}
					}
					else
					{
						Block& block = world.GetBlock( cx * X_SIZE + x + ix, y, cz * Z_SIZE + z + iz );
						if ( block.IsTransparent() )
						{
							facemask |= faces[ face ];

							const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
							faceColors[ colorFaces[ face ] ].Set( brightness, brightness, brightness );
						}
						face++;
					}
				}
			}
			

			/*
				bool right  = x + 1 < X_SIZE;
				bool left   = x - 1 >= 0;
				bool top    = y + 1 < Y_SIZE;
				bool bottom = y - 1 >= 0;
				bool front  = z + 1 < Z_SIZE;
				bool back   = z - 1 >= 0;

#define VALIDATE_SIDE( SIDE, X, Z )\
	if ( !SIDE ){ \
	SIDE = world.GetBlock( ((X) == 1 ? 0 : (X) == -1 ? (X_SIZE-1) : x), y, ((Z) == 1 ? 0 : (Z) == -1 ? (Z_SIZE-1) : z) ).IsTransparent(); }\
	else SIDE = GetBlock( x+X, y, z+Z ).IsTransparent();

				VALIDATE_SIDE( right, 1,  0 );
				VALIDATE_SIDE( left, -1,  0 );
				VALIDATE_SIDE( front, 0,  1 );
				VALIDATE_SIDE( back,  0, -1 );

#undef VALIDATE_SIDE
				
				if ( right ) facemask |= Right;
				if ( left  ) facemask |= Left;
				if ( front ) facemask |= Front;
				if ( back  ) facemask |= Back;
				if ( top    && world.GetBlock( cx * X_SIZE + x, y+1, cz * Z_SIZE + z ).IsTransparent() ) facemask |= Top;
				if ( bottom && world.GetBlock( cx * X_SIZE + x, y-1, cz * Z_SIZE + z ).IsTransparent() ) facemask |= Bottom;

				Vec3f faceColors[6];
				
				if ( right )
				{
					Block& block = GetBlock( x+1, y, z );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 1 ].Set( brightness, brightness, brightness );
					}
				}
				if ( left  )
				{
					Block& block = GetBlock( x-1, y, z );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 3 ].Set( brightness, brightness, brightness );
					}
				}
				if ( front )
				{
					Block& block = GetBlock( x, y, z+1 );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 0 ].Set( brightness, brightness, brightness );
					}
				}
				if ( back  )
				{
					Block& block = GetBlock( x, y, z-1 );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 5 ].Set( brightness, brightness, brightness );
					}
				}
				if ( top    )
				{
					Block& block = GetBlock( x, y+1, z );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 2 ].Set( brightness, brightness, brightness );
					}
				}
				if ( bottom )
				{
					Block& block = GetBlock( x, y-1, z );
					if ( block.IsTransparent() )
					{
						const float brightness = lerp( 0.1f, 1.0f, block.light / static_cast< float >( LIGHT_MAX ) );
						faceColors[ 4 ].Set( brightness, brightness, brightness );
					}
				}
				*/
				EmitCubeFaces( vertexElements, facemask, b.GetType(), translation, faceColors );
		}
	});

	// Generate VBO
	
	if ( vertexElements.empty() == false )
	{
		while ( renderLock ) ;
		renderLock = true;

		DestroyRenderData();
		GLuint tempVBO = 0;
		glGenBuffers( 1, &tempVBO );
		glBindBuffer( GL_ARRAY_BUFFER, tempVBO );

		// Feed vertex data to VBO
		glBufferData( GL_ARRAY_BUFFER,
			sizeof(vertexElements.front()) * vertexElements.size(),
			vertexElements.data(), GL_STATIC_DRAW );

		glFlush();

		numVerts = vertexElements.size() / NUM_FLOATS_PER_VERTEX;
		assert( vertexElements.size() % NUM_FLOATS_PER_VERTEX == 0 );

		idVBO = tempVBO;

		renderLock = false;
	}
	else
	{
		lock = false;
		return false;
	}
	Print( "Done\n" );
	lock = false;
	return true;
}

void Chunk::DestroyRenderData()
{
	if ( idVBO )
	{
		glDeleteBuffers( 1, &idVBO );
		idVBO = 0;
		numVerts = 0;
	}
}

void Chunk::GetNeighbors( const Vec3i& origin, std::vector< const Block* >& neighbors )
{
	World& world = World::GetInstance();
	for ( int face = 0; face < 6; ++face )
	{
		Vec3f normal = Block::CUBE_REF.GetCubeNormal( face );

		neighbors.push_back( &world.GetBlock( origin + normal ) );
	}
}