/*
 * Author      : Matthew Johnson
 * Date        : 08/Nov/2012
 * Description :
 *   The super amazing entry point of the application.
 */
//#include "glut.h"
#include "gl.h"

#include "main.h"
#include "World.h"
#include "InputController.h"

#include "Renderer.h"
#include "TextureFactory.h"

using namespace mzj;

void timer( int id );
void updateFrame();
void onResize( int width, int height );
void onKeyDown( unsigned char code, int x, int y );
void onKeyUp( unsigned char code, int x, int y );
void onSpecialDown( int key, int x, int y );
void onSpecialUp( int key, int x, int y );
void onMouseButton( int button, int state, int x, int y );
void onMouseDrag( int x, int y );
void onMouseMove( int x, int y );
void onIdle();
void onFocus( int focus );

void RenderAxis( float length = 10.0f );
void RenderCrosshair( float size=32.0f, float yOffset=0.0f );

bool wireframe = false;
bool pinMouse = false;

static const int FPS = 60;

unsigned int shader;

int main( int argc, char** argv )
{
	// Initialize glut
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(	WIN_WIDTH, WIN_HEIGHT );
	glutCreateWindow( WIN_TITLE );
	glutSetCursor( GLUT_CURSOR_NONE );

	glewInit();

	// Set default render state
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0, WIN_ASPECT, 0.1, 1000.0 );

	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );

	// Setup callbacks
	glutDisplayFunc      ( updateFrame   );
	glutReshapeFunc      ( onResize      );
	glutKeyboardFunc     ( onKeyDown     );
	glutKeyboardUpFunc   ( onKeyUp       );
	glutSpecialFunc      ( onSpecialDown );
	glutSpecialUpFunc    ( onSpecialUp   );
	glutMouseFunc        ( onMouseButton );
	glutMotionFunc       ( onMouseDrag   );
	glutPassiveMotionFunc( onMouseMove   );
	glutTimerFunc        ( FPS, timer, 0 );
	glutIdleFunc         ( onIdle        );
	glutVisibilityFunc   ( onFocus       ); 

	// Start glut
	glutMainLoop();

	return 0;
}

void timer( int id )
{
	//updateFrame();
	glutTimerFunc( FPS, timer, 0 );
	glutPostRedisplay();
}

void onIdle()
{
	static int last = 0;
	static int frames = 0;

	glutPostRedisplay();
	
	int curTime = glutGet( GLUT_ELAPSED_TIME );
	frames++;

	if ( curTime - last >= 1000 )
	{
		std::stringstream ss;
		ss << WIN_TITLE << " FPS: " << frames;
		glutSetWindowTitle( ss.str().c_str() );
		frames = 0;
		last = curTime;
	}
}

void updateFrame()
{
	// The world
	World& world = World::GetInstance();

	// Update input
	InputController::GetInstance().UpdateInput();

	// Pin mouse to center of screen
	if ( pinMouse )
		glutWarpPointer( WIN_WIDTH / 2, WIN_HEIGHT / 2 );

	// Clear buffer
	glClearColor( 0.1f, 0.1f, 0.15f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float time = 8.0f;
	static int d = 1;

	// Sky background
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	Player p = world.GetPlayer();

	Vec3f facing = p.playerGetFacingVector().Normalize();
	Vec3f orientation = p.GetRotation();

	glBegin( GL_QUADS );
	//white color
	glColor3f( time/12.0f+0.2f, time/12.0f+0.2f, time/12.0f+0.2f );
	glVertex2f( -1.0f, -1.0f );
	glVertex2f( 1.0f, -1.0f );
	//blue color
	glColor3f ( 0.0f, 0.0f, time/12.0f );
	glVertex2f( 1.0f,  1.0f );
	glVertex2f( -1.0f, 1.0f );

	glEnd();

	time += 0.01f * d;

	if ( time > 12 || time < 0 ) d = -d;

	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// Get ready to draw stuff
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	/*********** Update ************/
	
	world.UpdateDynamics( 1.0 / 60.0 );
	
	/*********** Render ************/

	glLoadMatrixf( world.GetTransformMatrix().GetArray() );
	glLineWidth( 3 );
	RenderAxis();

	world.Render();

	RenderCrosshair( 32.0f );
	
	glutSwapBuffers();
	glutPostRedisplay();
}

void onResize( int width, int height )
{
	glViewport( 0, 0, width, height );
#if 0 // TODO enforce aspect ratio
	const float newAspect = static_cast< float >( width ) / height;
	if ( std::abs( WIN_ASPECT - newAspect ) < 0.001f )
	{
		// Aspect is ok
		glViewport( 0, 0, width, height );
	}
	else
	{
		glutReshapeWindow( height * WIN_ASPECT , height);
	}
#endif
}

void onKeyDown( unsigned char code, int x, int y )
{
	InputController::GetInstance().UpdateKeyState( code, true );
}

void onKeyUp( unsigned char code, int x, int y )
{
	InputController::GetInstance().UpdateKeyState( code, false );

	if ( code == 'p' ) pinMouse = !pinMouse;
}

void onSpecialDown( int key, int x, int y )
{
	InputController::GetInstance().UpdateKeyState( key, true );
}

void onSpecialUp( int key, int x, int y )
{
	InputController::GetInstance().UpdateKeyState( key, false );
	switch ( key )
	{
		case GLUT_KEY_F4:
				exit( 0 );
			break;
		case GLUT_KEY_F3:
			wireframe = !wireframe;
			if ( wireframe )
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			else
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			break;
		case GLUT_KEY_F6:
			pinMouse = !pinMouse;
			break;
	}
}

void onMouseButton( int button, int state, int x, int y )
{
	InputController::GetInstance().UpdateKeyState( button, state==GLUT_DOWN );
}

void onMouseDrag( int x, int y )
{
	InputController::GetInstance().UpdateMouse( x, y );
}

 void onMouseMove( int x, int y )
{
	InputController::GetInstance().UpdateMouse( x, y );
}

 void RenderAxis( float length )
 {
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_LIGHTING );

	glBegin( GL_LINES );
	{
		glColor3f(   1,  0,  0 );
		glVertex3f(  0,  0,  0 );

		glColor3f(   1,  0,  0 );
		glVertex3f(  length,  0,  0 );

		glColor3f(   0,  1,  0 );
		glVertex3f(  0,  0,  0 );

		glColor3f(   0,  1,  0 );
		glVertex3f(  0,  length,  0 );

		glColor3f(   0,  0,  1 );
		glVertex3f(  0,  0,  0 );

		glColor3f(   0,  0,  1 );
		glVertex3f(  0,  0,  length );
	}

	glEnd();

 }

 void RenderCrosshair( float size, float yOffset )
 {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WIN_WIDTH, 0, WIN_HEIGHT, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor3f( 1,1,1 );
	glTranslatef( WIN_WIDTH / 2, WIN_HEIGHT / 2 -yOffset, 0 );
	glLineWidth( 3.0f );
	glBegin( GL_LINES );
	glVertex2f( -size/2, 0 );
	glVertex2f( size/2-1, 0 );
	glVertex2f( 0, -size/2 );
	glVertex2f( 0, size/2-1 );
	glEnd();
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
 }

 void Text( int x, int y, const std::string& text, Color color )
 {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WIN_WIDTH, 0, WIN_HEIGHT, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glColor4fv( &color.r );
	glRasterPos2i(x,y);
	for (unsigned i=0; i < text.size(); i++)
	{
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, text[i] );
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
 }

 void onFocus( int focus )
 {
	 pinMouse = focus == GLUT_VISIBLE;
 }