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

/* macros */

/* types */
typedef struct {
	unsigned int texture;
	int type;
} GFXtexture;

typedef struct {
	unsigned int* 	shader;
	unsigned int	program;
} GFXshader;

/* window */
GLFWwindow* gfxQuickWindowCreate(int width, int height, const char* title); // Implement monitor stuff later maybe? Open in a certain monitor by default? Idk??
GLFWwindow* gfxGetActiveWindow();

/* draw commands */
void gfxDrawClear(float r, float g, float b, float a);
void gfxDrawMode(GLenum mode);
void gfxQuickDrawSprite();
void gfxDrawMesh();
void gfxSetTexture(GFXtexture texture);
void gfxSetShader();
void gfxGetShader();
void gfxSetBlendmode();


/* default callbacks */
void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window);
void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height);

#endif