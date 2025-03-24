#include "gfx.h"

/* variables */
GLFWwindow* ACTIVE_WINDOW = NULL;

GLFWwindow* gfxQuickWindowCreate(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		printf("[E]\t In window_create(): Failed to open window :("); // REPLACE ALL OF THESE WITH LOG EVENTUALLY
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, &_DEFAULT_FRAMEBUFFER_CALLBACK);
	glfwSetWindowCloseCallback(window, &_DEFAULT_WINDOW_CLOSE_CALLBACK);

	// TODO: what in the goddamn fuck is this notation
	// (I made this note ages ago and now its just a beautiful artifact. Its called a typecast hun)
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initialize glad :(");
	}

	ACTIVE_WINDOW = window;
	return ACTIVE_WINDOW;
}

GLFWwindow* gfxGetActiveWindow() {
	return ACTIVE_WINDOW;
}

void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window) {
	//glfwSetWindowShouldClose(window, 1);
}

void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height) {
	//glViewport(0, 0, width, height); // Directly scales window when resized :3
}
