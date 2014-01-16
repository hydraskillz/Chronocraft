#pragma once

#ifdef _WIN32
#	include <glew.h>
#endif
#include <glut.h>
/*
// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);
*/
/*
// VBO Extension Function Pointers
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;					// VBO Name Generation Procedure
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;					// VBO Bind Procedure
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;					// VBO Data Loading Procedure
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;			// VBO Deletion Procedure
*/
/*
// VBO Extension Function Pointers
__declspec( selectany ) PFNGLGENBUFFERSARBPROC glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
__declspec( selectany ) PFNGLBINDBUFFERARBPROC glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
__declspec( selectany ) PFNGLBUFFERDATAARBPROC glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
__declspec( selectany ) PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");

*/