#ifndef BAPHOMET_GFX_H
#define BAPHOMET_GFX_H

/*
*	gfx.h
*	Created: homestuck day
*	Description: Graphical oddities and curios alike :3
*	Shout out to https://github.com/nothings/stb
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
#include <stb/stb_image.h>

#include "util.h"

/* macros */

/* vertex formats*/
#define VERTEX_FORMAT_XYZ	0x0
#define VERTEX_FORMAT_UV	0x1 << 0
#define VERTEX_FORMAT_RBG	0x1 << 1
#define VERTEX_FORMAT_ALPHA	0x1 << 2

/* types */
typedef struct {
	unsigned int texture;
	int type;
} GFXtexture;

typedef struct {
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
void gfxDrawMode(GLenum mode);
void gfxQuickDrawSprite();
void gfxDrawMesh(GFXmesh* mesh);
void gfxSetTexture(GFXtexture* texture);
void gfxSetBlendmode();

/* mesh primitives */ 
GFXmesh gfxMeshStart(int format);
void gfxMeshAddVertices(GFXmesh* mesh, GLenum usage_mode, float* vertices, size_t nv, int* indices, size_t ni);
void gfxMeshFinish(GFXmesh* mesh);
GFXmesh gfxGenerateTri(float b, float h);
GFXmesh gfxGenerateRect(float w, float h);
GFXmesh gfxGenerateCirc(float r, float subdiv);

/* shaders */
void gfxSetShader(GFXshader* shader);
GFXshader* gfxGetShader();
GFXshader gfxQuickCreateShader(char* vshader_fp, char* fshader_fp);
GFXshader gfxCreateShader(GFXsfrag* fragments, int n_frags); 
GFXsfrag gfxBuildShaderFragment(char* filepath, GLenum shader_type);
void gfxShaderSetUniformVec2(GFXshader* shader, char* name, float x, float y); // TODO

/* default callbacks */
void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window);
void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height);

#endif