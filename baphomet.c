/* See LICENSE file for copyright and license details
*
*	Baphomet is a simple C game-backend writted using libSDL for the WW
*	project as well as for learning. Generally, the design philosophy is
*	to be acessable and fun. So, yeah :3. Code design is roughly based on
* 	work in the dwm window manager (I just have those files on hand and
* 	they seem pretty).
*
*/

/* macros */
// NONE YET

/* includes */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <argp.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* core includes */
#include "util.h"
#include "gfx.h"
#include "ring.h"

#include "assets/sinners/default.h"
#include "assets/sinners/kitty.h"

/* variables */
//static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;

/* configuration, following dwm, allows access to above variables */
#include "config.h"

/* function defines (idk what its called) */
static void err(char* error_text, int n, ...);
void _DEFAULT_GLFW_ERROR_CALLBACK(int error, const char* desc);

/* functions */

struct commandline_args {int debug, quiet; };
static error_t parse_opt(int key, char* arg, struct argp_state* state) {
	struct commandline_args* arguments = state->input;
	switch(key) {
    case 'v': case 'd':
      arguments->debug = 1;
      break;
    case 'q':
      arguments->quiet = 1;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        argp_usage(state);
    case ARGP_KEY_END:
      if (state->arg_num >= 2)
        argp_usage(state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

struct argp argp = { argp_program_options, parse_opt, argp_program_args_desc, argp_program_desc };
int main(int argc, char* argv[]) {
	struct commandline_args cmd_args = {.debug = 0, .quiet = 0};

	// Parse Command Arguments
	argp_parse(&argp, argc, argv, 0, 0, &cmd_args);
	(cmd_args.debug) ? fprintf(stderr,"\tDEBUG ENABLED:\n"):0;

	if (!glfwInit())
		printf("[E]\t In main(): Failed to initialize glfw :("); // REPLACE ALL OF THESE WITH LOG EVENTUALLY

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	ringInit();

	ringAppendSins(&(sin_default.oid), 1);
	ringAppendSins(&(sin_kitty.oid), 1);

	ringExecuteEvent(EVENT_CREATE);

	ringExecuteEvent(EVENT_STEP);

	ringCleanup();

	gfxQuickWindowCreate(default_window_width, default_window_height, default_window_name);
	glViewport(0, 0, default_window_width, default_window_height);	// Create a window and viewport of the windows size

	GFXcamera cam = gfxCreateCamera();
	gfxSetCamera(&cam);

	vec3 from = {0.05, 0.0, -0.8};
	vec3 to = {0.0, 0.0, 0.0};
	gfxCameraSetViewLookat(&cam, from, to);
	gfxCameraSetProjectionPersp(&cam, 210.0 * 3.14 / 360.0, 1.0, 0.0001, 10000.0);

	GFXtexture tex_moon = gfxLoadTexture("assets/sprites/moon.png", GL_RGBA);

	GFXtexture tex_sun = gfxLoadTexture("assets/sprites/sun.png", GL_RGBA);
	gfxSetTexture(&tex_sun);

	GFXshader default_shader = gfxQuickCreateShader("assets/shaders/texture.vsh","assets/shaders/texture.fsh");
	gfxSetShader(&default_shader);

  GFXmesh mesh = gfxGenerateRect(1.0f, 1.0f);

  GFXmesh mesh_moon = gfxGenerateRect(0.5f, 0.5f);

  glUseProgram(gfxGetShader()->program);

	double PROGRAM_TIME = glfwGetTime();
	while (!glfwWindowShouldClose(gfxGetActiveWindow())) {
		double DELTA_TIME = PROGRAM_TIME - glfwGetTime();

		//glClearColor(0.7176f, 0.1098f, 0.1098f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		gfxShaderSetUniformVec2(gfxGetShader(), "position", sin(glfwGetTime()*1.2312f)*1.0f, cos(glfwGetTime()*1.2312f)*1.0f);

		if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
		gfxShaderSetUniformVec2(gfxGetShader(), "position", sin(glfwGetTime()*15.0f)*1.0f, cos(glfwGetTime()*15.0f)*1.0f);
		}

		gfxSetTexture(&tex_moon);
		gfxDrawMesh(&mesh_moon);

		gfxShaderSetUniformVec2(gfxGetShader(), "position", 0.0f, 0.0f);

		gfxSetTexture(&tex_sun);
		gfxDrawMesh(&mesh);

		//gfxCameraSetProjectionPersp(&cam, 3.14/2.0 + (cos(glfwGetTime()) * 3.14/2), 1.0, 0.0001, 10000.0);

		vec3 from = {sin(glfwGetTime()) * 1.5, 0.0, cos(glfwGetTime()) * 1.5};
		gfxCameraSetViewLookat(&cam, from, to);

		gfxShaderSetUniformMat4(gfxGetShader(), "view", gfxGetCamera()->view_mat);
		gfxShaderSetUniformMat4(gfxGetShader(), "projec", gfxGetCamera()->proj_mat);

		glfwSwapBuffers(gfxGetActiveWindow());
		glfwPollEvents();

	if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(gfxGetActiveWindow(), GL_TRUE);
    }
	}
	
	glfwTerminate();
	return 0;
}

static void err(char* error_text, int n, ...) {
	char output_log[512];
	sprintf(output_log, error_log_format, error_text, glGetError());
	fprintf(stderr, "%s", output_log);
}

void _DEFAULT_GLFW_ERROR_CALLBACK(int error, const char* desc) {
	char* err_str;
	sprintf(err_str, "%d", error);
	err("GLFW Error Callback Triggered", 2, "error code", err_str, "desc", desc);
}
