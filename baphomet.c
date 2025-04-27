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

/* sinner index */
#include "assets/sinners/sinner-index.h"

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

	ringAppendSins((Sin**)&(sin_solar.oid), 1);
	ringAppendSins((Sin**)&(sin_camera.oid), 1);

	gfxQuickWindowCreate(default_window_width, default_window_height, default_window_name);
	glViewport(0, 0, default_window_width, default_window_height);	// Create a window and viewport of the windows size

	ringExecuteEvent(EVENT_CREATE);

	GFXshader default_shader = gfxQuickCreateShader("assets/shaders/texture.vsh","assets/shaders/texture.fsh");
	gfxSetShader(&default_shader);

  glUseProgram(gfxGetShader()->program);

	double PROGRAM_TIME = glfwGetTime();
	while (!glfwWindowShouldClose(gfxGetActiveWindow())) {
		double DELTA_TIME = PROGRAM_TIME - glfwGetTime();

		//glClearColor(0.7176f, 0.1098f, 0.1098f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ringExecuteEvent(EVENT_STEP);

		gfxShaderSetUniformMat4(gfxGetShader(), "view", gfxGetCamera()->view_mat);
		gfxShaderSetUniformMat4(gfxGetShader(), "projec", gfxGetCamera()->proj_mat);

		glfwSwapBuffers(gfxGetActiveWindow());
		glfwPollEvents();

	if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(gfxGetActiveWindow(), GL_TRUE);
    }
	}
	
	ringExecuteEvent(EVENT_DESTROY);
	ringCleanup();
	glfwTerminate();
	return 0;
}

static void err(char* error_text, int n, ...) {
	char output_log[512];
	sprintf(output_log, error_log_format, error_text, glGetError()); // use sprintf_s here
	fprintf(stderr, "%s", output_log);
}

void _DEFAULT_GLFW_ERROR_CALLBACK(int error, const char* desc) {
	char* err_str;
	sprintf(err_str, "%d", error);
	err("GLFW Error Callback Triggered", 2, "error code", err_str, "desc", desc);
}
