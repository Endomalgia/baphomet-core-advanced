#include "gfx.h"

/* stb image funcions */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* variables */
GLFWwindow* ACTIVE_WINDOW = NULL;

GLFWwindow* gfxQuickWindowCreate(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
		printf("[E]\t In window_create(): Failed to open window :("); // REPLACE ALL OF THESE WITH LOG EVENTUALLY
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, &_DEFAULT_FRAMEBUFFER_CALLBACK);
	glfwSetWindowCloseCallback(window, &_DEFAULT_WINDOW_CLOSE_CALLBACK);

	// TODO: what in the goddamn fuck is this notation
	// (I made this note ages ago and now its just a beautiful artifact. Its called a typecast hun)
	// ((I said this before spending the next several real life hours crying over a linking error before realizing that I had forgotten to add glad.c to the project. I will be nicer to my past self from now on I am sorry))
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initialize glad :(");
	}

	// REMOVE DEBUG
	fprintf(stderr, "RESULT OF FUNCTION: %x\n",stbi_is_16_bit("assets/sprites/baphomet.png"));

	ACTIVE_WINDOW = window;
	return ACTIVE_WINDOW;
}

GLFWwindow* gfxGetActiveWindow() {
	return ACTIVE_WINDOW;
}

GFXmesh gfxGenerateRect(float w, float h) {
	GFXmesh mesh = gfxMeshStart(VERTEX_FORMAT_UV);
	float vert[4*5] = {
		-w/2,	 h/2, 0.0f, 0.0f, 1.0f,
		 w/2,	 h/2, 0.0f, 1.0f, 1.0f,
		 w/2,	-h/2, 0.0f, 1.0f, 0.0f,
		-w/2,	-h/2, 0.0f, 0.0f, 0.0f};
	int indcs[6] = {3,0,1,1,2,3};
	gfxMeshAddVertices(mesh, GL_STATIC_DRAW, vert, 4*5, indcs, 6);
	gfxMeshFinish(mesh);
	return mesh;
}

GFXmesh gfxMeshStart(int format) {
	GFXmesh mesh = {0, 0, format};
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	return mesh;
}

void gfxMeshAddVertices(GFXmesh mesh, GLenum usage_mode, float* vertices, size_t nv, int* indices, size_t ni) {
	unsigned int vbo, ebo, nelem;
	nelem = 3 + (mesh.format&VERTEX_FORMAT_UV)*2 + (mesh.format&VERTEX_FORMAT_RBG)*3 + (mesh.format&VERTEX_FORMAT_ALPHA); // Fight me cowards
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nv * mesh.format, vertices, usage_mode);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * ni, indices, usage_mode);
	mesh.elements = ni;
}

void gfxMeshFinish(GFXmesh mesh) {
	unsigned int stride = 3 + (mesh.format&VERTEX_FORMAT_UV)*2 + (mesh.format&VERTEX_FORMAT_RBG)*3 + (mesh.format&VERTEX_FORMAT_ALPHA);
	int index;
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)0);
	glEnableVertexAttribArray(index);
	if (mesh.format&VERTEX_FORMAT_UV)
		glVertexAttribPointer(index++, 2, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(index);
	if (mesh.format&VERTEX_FORMAT_RBG)
		glVertexAttribPointer(index++, 3+(mesh.format&VERTEX_FORMAT_ALPHA), GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)((3+(mesh.format&VERTEX_FORMAT_UV)*2)*sizeof(float)));
		glEnableVertexAttribArray(index);
}

GFXshader gfxCreateShader(GFXsfrag* fragments) {

}

GFXsfrag gfxBuildShaderFragment(char* filepath, int type) {
	GLchar* shader_source;
	char info_log[512];
	int success;

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&SHADERSOURCE, NULL);
	glCompileShader(shader);
}

void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, 1);
}

void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); // Directly scales window when resized :3
}
