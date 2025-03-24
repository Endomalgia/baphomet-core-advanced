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
#include "gfx.h"
#include "ring.h"

/* variables */
//static SDL_Window* window = NULL;
//static SDL_Renderer* renderer = NULL;

/* configuration, following dwm, allows access to above variables */
#include "config.h"

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

	gfxQuickWindowCreate(default_window_width, default_window_height, default_window_name);
	
	glViewport(0, 0, default_window_width, default_window_height);	// Create a window and viewport of the windows size

	double PROGRAM_TIME = glfwGetTime();
	while (!glfwWindowShouldClose(gfxGetActiveWindow())) {
		double DELTA_TIME = PROGRAM_TIME - glfwGetTime();

		glClearColor(0.7176f, 0.1098f, 0.1098f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(gfxGetActiveWindow());
		glfwPollEvents();

		if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {

      glfwSetWindowShouldClose(gfxGetActiveWindow(), GL_TRUE);
    }
	}
	
	glfwTerminate();

ERROR:
	// None yet :3
}

/* THIS WOULD BE NICE
static error_t log(char* error_text, int line_mac, int file_mac, ...) {
	#ifndef BAPHOMET_ALLOW_DEBUG
		return 0;
	#endif

	va_list log_valist



	va_start(log_valist, num)


}*/
