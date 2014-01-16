#include "Cube.h"
#include "gl.h"

using namespace mzj;

Cube::Cube( float size )
	: size( size )
{
	BuildVerts();
	// Setup normals
	//normals = new Vec3f[36];
	Vec3f _normals[] =
	{
		Vec3f( 0, 0, 1 ),
		Vec3f( 1, 0, 0 ),
		Vec3f( 0, 1, 0 ),
		Vec3f( -1, 0, 0 ),
		Vec3f( 0, -1, 0 ),
		Vec3f( 0, 0, -1 )
	};
	// Set nornals
	for ( int i = 0; i < 36; ++i )
	{
		normals[i] = _normals[i/6];
	}

	// UV Coords for the cube
	unsigned int face = Front;
	while ( face <= Back )
	{
		// Front/Left
		if ( face == Front || face == Left )
		{
			int iface = (face == Front ? 0 : 3)*6;

			texCoords[iface  ].Set( 1, 0 );
			texCoords[iface+1].Set( 0, 0 );
			texCoords[iface+2].Set( 0, 1 );
			texCoords[iface+3].Set( 0, 1 );
			texCoords[iface+4].Set( 1, 1 );
			texCoords[iface+5].Set( 1, 0 );
		}

		// Back/Bottom
		else if ( face == Back || face == Bottom )
		{
			int iface = (face == Back ? 5 : 4)*6;

			texCoords[iface  ].Set( 1, 1 );
			texCoords[iface+1].Set( 0, 1 );
			texCoords[iface+2].Set( 0, 0 );
			texCoords[iface+3].Set( 0, 0 );
			texCoords[iface+4].Set( 1, 0 );
			texCoords[iface+5].Set( 1, 1 );
		}
		// Right/Top
		else if ( face == Right || face == Top )
		{
			int iface = (face == Right ? 1 : 2)*6;

			texCoords[iface  ].Set( 0, 0 );
			texCoords[iface+1].Set( 0, 1 );
			texCoords[iface+2].Set( 1, 1 );
			texCoords[iface+3].Set( 1, 1 );
			texCoords[iface+4].Set( 1, 0 );
			texCoords[iface+5].Set( 0, 0 );
		}
		
		face <<= 1;
	}
}

Cube::~Cube()
{}

void Cube::BuildVerts()
{
	// Setup verticies
	//Vec3f* verts = new Vec3f[36];

	float s = size / 2.0f;
	// Front
	verts[0].Set(  s,  s,  s );
	verts[1].Set( -s,  s,  s );
	verts[2].Set( -s, -s,  s );

	verts[3].Set( -s, -s,  s );
	verts[4].Set(  s, -s,  s );
	verts[5].Set(  s,  s,  s );

	// Right
	verts[6].Set(  s,  s,  s );
	verts[7].Set(  s, -s,  s );
	verts[8].Set(  s, -s, -s );

	verts[ 9].Set(  s, -s, -s );
	verts[10].Set(  s,  s, -s );
	verts[11].Set(  s,  s,  s );
	
	// Top
	verts[12].Set(  s,  s,  s );
	verts[13].Set(  s,  s, -s );
	verts[14].Set( -s,  s, -s );

	verts[15].Set( -s,  s, -s );
	verts[16].Set( -s,  s,  s );
	verts[17].Set(  s,  s,  s );

	// Left
	verts[18].Set( -s,  s,  s );
	verts[19].Set( -s,  s, -s );
	verts[20].Set( -s, -s, -s );

	verts[21].Set( -s, -s, -s );
	verts[22].Set( -s, -s,  s );
	verts[23].Set( -s,  s,  s );

	// Bottom
	verts[24].Set( -s, -s, -s );
	verts[25].Set(  s, -s, -s );
	verts[26].Set(  s, -s,  s );

	verts[27].Set(  s, -s,  s );
	verts[28].Set( -s, -s,  s );
	verts[29].Set( -s, -s, -s );

	// Back
	verts[30].Set(  s, -s, -s );
	verts[31].Set( -s, -s, -s );
	verts[32].Set( -s,  s, -s );

	verts[33].Set( -s,  s, -s );
	verts[34].Set(  s,  s, -s );
	verts[35].Set(  s, -s, -s );
	/*
	unsigned int VBO_Verts;
	// glSubBufferData
	// Generate And Bind The Vertex Buffer
	glGenBuffersARB( 1, &VBO_Verts );							// Get A Valid Name
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, VBO_Verts );			// Bind The Buffer
	// Load The Data
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, 36*3*sizeof(float), verts, GL_STATIC_DRAW_ARB );
	delete[] verts;
	return VBO_Verts;
	*/
}