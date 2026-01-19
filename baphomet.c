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
#include <portaudio.h>

#include <unistd.h>

/* core includes */
#include "util.h"
#include "gfx.h"
#include "ring.h"
#include "aud.h"

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
		fprintf(stderr, "[E]\t In main(): Failed to initialize glfw :(\n"); // REPLACE ALL OF THESE WITH LOG EVENTUALLY

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	FT_Library ft_l; // Cant be a ptr so freetype can allocate it lolz
	if (FT_Init_FreeType(&ft_l)) {
		printf("[E]\t In main(): Failed to initialize FreeType :(\n");
		exit(0);
	}
	gfxSetActiveFTLibrary(&ft_l);

	PaError err = Pa_Initialize();
	if (err != paNoError)
		fprintf(stderr, "[E]\t In main(): Failed to initialize portaudio :( : %s\n", Pa_GetErrorText(err));

	PaStreamParameters in = {0, 0, DEFAULT_SAMPLE_FORMAT, 0, NULL};
	PaStreamParameters out = {0, 0, DEFAULT_SAMPLE_FORMAT, 0, NULL};
	audSetInputParams(&in);
	audSetOutputParams(&out);

	// Print all audio devices
	char** log = audGetDeviceList();
	fprintf(stderr, "Devices : \n");
	for(int i=0;log[i]!=NULL;i++) {
		fprintf(stderr, "\t (%u) %s\n", (unsigned int)i, log[i]);
	}
	free(log);

	audSetInputDevice(Pa_GetDefaultInputDevice());
	audSetOutputDevice(Pa_GetDefaultOutputDevice());

	ringInit();

	static Sin* room_solar[] = {&sin_solar, &sin_camera};
	ringAppendSins(room_solar, 2);

	gfxQuickWindowCreate(default_window_width, default_window_height, default_window_name);
	glViewport(0, 0, default_window_width, default_window_height);	// Create a window and viewport of the windows size

	/* Generate mesh primitives */
	gfxGeneratePrimitives();

	ringExecuteEvent(EVENT_CREATE);

	GFXfont f_silbi = gfxLoadFont("assets/fonts/nerd.ttf");

	GFXshader default_shader = gfxQuickCreateShader("assets/shaders/texture.vsh","assets/shaders/texture.fsh");
	gfxSetShader(&default_shader);

  glUseProgram(gfxGetShader()->program);

  AUDsfx* wave = audLoadSfx("assets/sfx/kernel.wav");
  //audSoundPlay(wave, false);

	int fps_mon_index = 0;
	int fps_mon[50];

	double FRAME_TIME = 1.0 / 30.0; // 30fps
	double PROGRAM_TIME = glfwGetTime();
	double DELTA_TIME = 0;
	while (!glfwWindowShouldClose(gfxGetActiveWindow())) {

		//glClearColor(0.7176f, 0.1098f, 0.1098f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		gfxShaderSetUniformVec3(gfxGetShader(), "scale", 1.0f, 1.0f, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ringExecuteEvent(EVENT_STEP);

		gfxShaderSetUniformMat4(gfxGetShader(), "view", GLM_MAT4_IDENTITY);
		gfxShaderSetUniformMat4(gfxGetShader(), "projec", GLM_MAT4_IDENTITY);


		float f_in = 0.5f;
		float f_sp = 0.4f;
		gfxDrawText(&f_silbi, "Goodmorning!\0", -0.775f, f_in + 0.1, 0.002f);
		gfxDrawText(&f_silbi, "How's\0", -0.775f, f_in-f_sp*1, 0.005f);
		gfxDrawText(&f_silbi, "your\0", -0.6f, f_in-f_sp*2, 0.005f);
		gfxDrawText(&f_silbi, "day?\0", -0.6f, f_in-f_sp*3, 0.005f);

		gfxShaderSetUniformMat4(gfxGetShader(), "view", gfxGetCamera()->view_mat);
		gfxShaderSetUniformMat4(gfxGetShader(), "projec", gfxGetCamera()->proj_mat);

		glfwSwapBuffers(gfxGetActiveWindow());
		glfwPollEvents();
		if (glfwGetKey(gfxGetActiveWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(gfxGetActiveWindow(), GL_TRUE);
    }

		double TIME = glfwGetTime();
		double DELTA_TIME = TIME - PROGRAM_TIME;
		PROGRAM_TIME = TIME;

		/*
		if (DELTA_TIME < FRAME_TIME) {
			usleep((FRAME_TIME - DELTA_TIME) * 1000000l);
			DELTA_TIME = glfwGetTime() - PROGRAM_TIME;
		}
		*/

		fps_mon_index++;
		if (fps_mon_index >= 50) {
			fps_mon_index = 0;
		}
		fps_mon[fps_mon_index] = (int)(1.0f/DELTA_TIME);

		int avg_fps = 0;
		for (int i=0; i<50; i++) {
			avg_fps += fps_mon[i];
		}
		avg_fps /= 50;

		printf("(%f) [dt] = %f (%d fps)\n",glfwGetTime(),DELTA_TIME,avg_fps);
	}

	ringExecuteEvent(EVENT_DESTROY);

	ringCleanup();

	Pa_Terminate();

	FT_Done_FreeType(ft_l); // Technically FT stops being needed after the fonts are loaded btw

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
