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
#define SDL_MAIN_USE_CALLBACKS 	TRUE

/* includes */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* Core */
//#include "ring.h"
//#include "draw.h"

/* variables */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

/* configuration, following dwm, allows acess to above variables */
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
SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[]) {
	struct commandline_args cmd_args = {.debug = 0, .quiet = 0};

	// Parse Command Arguments
	argp_parse(&argp, argc, argv, 0, 0, &cmd_args);
	(cmd_args.debug) ? fprintf(stderr,"\tDEBUG ENABLED:\n"):0;

	SDL_SetAppMetadata(program_name, argp_program_version, program_identifier);

	if (!SDL_Init(SDL_INIT_VIDEO))
		//log("SDL_Init( %x )",  __LINE__, __FILE__, SDL_INIT_VIDEO);
		goto SDL_ERROR;

	if (!SDL_CreateWindowAndRenderer("CORE/renderer/clear", default_window_width, default_window_height, 0, &window, &renderer))
		goto SDL_ERROR;

	return SDL_APP_CONTINUE;

SDL_ERROR:
	SDL_Log("\n%s\n", SDL_GetError());
	exit(SDL_APP_FAILURE);
}


SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS;
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

	const double now = ((double)SDL_GetTicks()) / 1000.0;
	const float red = (float) (0.5 + 0.5 * SDL_sin(now));
	const float gre = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2/3));
	const float blu = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4/3));

	SDL_SetRenderDrawColorFloat(renderer, red, gre, blu, SDL_ALPHA_OPAQUE_FLOAT);

	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	// DONE AUTOMATICALLY, CONFIGURE IF DESIRED
}

/* THIS WOULD BE NICE
static error_t log(char* error_text, int line_mac, int file_mac, ...) {
	#ifndef BAPHOMET_ALLOW_DEBUG
		return 0;
	#endif

	va_list log_valist



	va_start(log_valist, num)


}*/