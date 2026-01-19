#ifndef BAPHOMET_GFX_H
#define BAPHOMET_GFX_H

/*
*	gfx.h
*	Created: homestuck day 2025
*	Description: Graphical oddities and curios alike :3
*	Shout out to https://github.com/nothings/stb
*                https://github.com/recp/cglm
* and the lovely https://learnopengl.com
*/

/* defines */


/* includes */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <argp.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stb/stb_image.h>

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>
#include <cglm/cam.h>

#include "util.h"

/* macros */

/* vertex formats*/
#define VERTEX_FORMAT_XYZ	0x0
#define VERTEX_FORMAT_UV	0x1 << 0
#define VERTEX_FORMAT_RGB	0x1 << 1
#define VERTEX_FORMAT_ALPHA	0x1 << 2

/* types */
typedef struct {
	unsigned int id;
	int width;
	int height;
	int n_channels;
} GFXtexture;

typedef struct {
	GFXtexture*  texture_map;
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
} GFXsprite;	// Part of a texture

typedef struct {
	GFXtexture 	 texture;
	int 		 bearingx;
	int 		 bearingy;
	unsigned int advance;
} GFXcharacter;

typedef struct {
	GFXcharacter* index;
} GFXfont;

typedef struct {
	mat4 proj_mat;
	mat4 view_mat;
} GFXcamera;

typedef struct {	// Put stride somewhere in here?
	unsigned int vao;
	unsigned int elements;
	int 		 format;
} GFXmesh;

typedef struct {
	unsigned int program;
} GFXshader;	// More things may be added to this, just typedef to int if otherwise.

typedef unsigned int GFXsfrag;

/* window */
GLFWwindow* gfxQuickWindowCreate(int width, int height, const char* title); // Implement monitor stuff later maybe? Open in a certain monitor by default? Idk??
GLFWwindow* gfxGetActiveWindow();

/* draw commands */
void gfxDrawClear(float r, float g, float b, float a);
void gfxDrawMesh(GFXmesh* mesh);
void gfxSetTexture(GFXtexture* texture);
//void gfxDrawSprite2D(GFXsprite* sprite, float x, float y, float depth, float rot, float sx, float sy);
//void gfxDrawSprite(GFXsprite* sprite, float x, float y, float z, float rot);
//void gfxDrawSpriteExt(GFXsprite* sprite, float x, float y, float z, float sx, float sy, float sz);
void gfxDrawTexture2D(GFXtexture* texture, float x, float y, float scale);
void gfxDrawTexture2DExt(GFXtexture* texture, float x, float y, float depth, float rot, float sx, float sy);

/* camera commands */
void gfxCameraSetViewLookat(GFXcamera* camera, vec3 from, vec3 to);
void gfxCameraSetViewAero(GFXcamera* camera, float x, float y, float z, float yaw, float pitch, float roll);
void gfxCameraSetViewGimb(GFXcamera* camera, float x, float y, float z, float s, float p, float d, float f);
void gfxCameraSetProjectionOrtho(GFXcamera* camera, float left, float right, float bottom, float top, float near, float far);
void gfxCameraSetProjectionPersp(GFXcamera* camera, float fov, float aspect, float near, float far); // fov expressed in rad between 0 and pi (for some reason??)
GFXcamera gfxCreateCamera();
void gfxApplyCamera(GFXcamera* camera);
GFXcamera* gfxGetCamera();
void gfxSetCamera(GFXcamera* camera);

/* mesh primitives */
GFXmesh gfxMeshStart(int format);
void gfxMeshAddVertices(GFXmesh* mesh, GLenum usage_mode, float* vertices, size_t nv, int* indices, size_t ni);
void gfxMeshFinish(GFXmesh* mesh);
GFXmesh gfxGenerateTri(float b, float h);
GFXmesh gfxGenerateRect(float w, float h);
GFXmesh gfxGenerateCirc(float r, float subdiv);

void gfxGeneratePrimitives();
GFXmesh gfxGetPrimitiveSquare();
GFXmesh gfxGetPrimitiveTriangle();

/* textures */
GFXtexture gfxLoadTexture(char* filepath, GLint format);
GFXtexture gfxLoadTextureCharArray(unsigned char* imgdata, GLint format, int width, int height, int n_channels);
void gfxUnloadTexture(GFXtexture* texture);

/* sprites */
GFXsprite gfxQuickCreateSprite(char* filepath, GLint format);
GFXsprite gfxCreateSprite(GFXtexture* texture_map, int x, int y, int width, int height);

/* shaders */
void gfxSetShader(GFXshader* shader);
GFXshader* gfxGetShader();
GFXshader gfxQuickCreateShader(char* vshader_fp, char* fshader_fp);
GFXshader gfxCreateShader(GFXsfrag* fragments, int n_frags);
GFXsfrag gfxBuildShaderFragment(char* filepath, GLenum shader_type);
void gfxShaderSetUniformVec2(GFXshader* shader, char* name, float x, float y);
void gfxShaderSetUniformVec3(GFXshader* shader, char* name, float x, float y, float z);
void gfxShaderSetUniformMat4(GFXshader* shader, char* name, mat4 matrix);

/* fonts */
void gfxSetActiveFTLibrary(FT_Library* ft_l);
FT_Library* gfxGetActiveFTLibrary();
GFXfont gfxLoadFont(char* filepath);

void gfxDrawText(GFXfont* font, char* text, float x, float y, float scale);

/*
void gfxLoadFont(char* filepath);
void gfxSetFont(FT_Face* font);
void gfxSetFontSize(FT_Face* font);
void gfxDrawText(float x, float y, char* text);
void gfxDrawTextExt(float x, float y, char* text, ...);
*/

// Have these input into draw functions instead of having variations for each one
//mat4 xydmat(float x, float y, float depth);
//mat4 xysrdmat(float x, float y, float sx, float sy, float rot, float depth);
//mat4 3dyprmat(float x, float y, float z, float sx, float sy, float sz, float yaw, float pitch, float roll);
//mat4 3deulmat(float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz, float rw);

/* default callbacks */
void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window);
void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height);

#endif
