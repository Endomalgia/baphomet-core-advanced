#include "gfx.h"

/* stb image funcions */
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

/* variables */
GLFWwindow* ACTIVE_WINDOW = 	NULL;
GFXshader* 	ACTIVE_SHADER = 	NULL;
GFXcamera*	ACTIVE_CAMERA = 	NULL;

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

	ACTIVE_WINDOW = window;
	return ACTIVE_WINDOW;
}

GLFWwindow* gfxGetActiveWindow() {
	return ACTIVE_WINDOW;
}

void gfxDrawMesh(GFXmesh* mesh) {
	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->elements, GL_UNSIGNED_INT, 0);
}

void gfxSetTexture(GFXtexture* tex) {
	glBindTexture(GL_TEXTURE_2D, tex->texture);
}

void gfxCameraSetViewLookat(GFXcamera* camera, vec3 from, vec3 to) { // redo with vectors
	vec3 up = {0.0f, 1.0f, 0.0f};
	glm_lookat(from, to, up, camera->view_mat);
}

//void gfxCameraSetViewAero(GFXcamera* camera, float x, float y, float z, float yaw, float pitch, float roll);
//void gfxCameraSetViewGimb(GFXcamera* camera, float x, float y, float z, float s, float p, float d, float f);

void gfxCameraSetProjectionOrtho(GFXcamera* camera, float left, float right, float bottom, float top, float near, float far) {
	glm_ortho(left, right, bottom, top, near, far, camera->proj_mat);
}

void gfxCameraSetProjectionPersp(GFXcamera* camera, float fov, float aspect, float near, float far) {
	glm_perspective(fov, aspect, near, far, camera->proj_mat);
}

GFXcamera gfxCreateCamera() {
	GFXcamera cam;
	return cam;
}

void gfxApplyCamera(GFXcamera* camera) {
	GFXshader* shd = gfxGetShader();
	GFXcamera* cam = gfxGetCamera();
	gfxShaderSetUniformMat4(shd, "view", cam->view_mat);
	gfxShaderSetUniformMat4(shd, "projec", cam->proj_mat);
}

GFXcamera* gfxGetCamera() {
	return ACTIVE_CAMERA;
}

void gfxSetCamera(GFXcamera* camera) {
	ACTIVE_CAMERA = camera;
}


GFXmesh gfxMeshStart(int format) {
	GFXmesh mesh = {0, 0, format};
	glGenVertexArrays(1, &mesh.vao);
	glBindVertexArray(mesh.vao);
	return mesh;
}

void gfxMeshAddVertices(GFXmesh* mesh, GLenum usage_mode, float* vertices, size_t nv, int* indices, size_t ni) {
	unsigned int vbo, ebo, nelem;
	nelem = 3 + !!(mesh->format&VERTEX_FORMAT_UV)*2 + !!(mesh->format&VERTEX_FORMAT_RGB)*3 + !!(mesh->format&VERTEX_FORMAT_ALPHA); // Fight me cowards
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nv * nelem, vertices, usage_mode);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ni, indices, usage_mode);
	mesh->elements += ni;
}

void gfxMeshFinish(GFXmesh* mesh) {
	unsigned int stride, index, uv, rgb, a;
	uv = !!(mesh->format & VERTEX_FORMAT_UV);
	rgb = !!(mesh->format & VERTEX_FORMAT_RGB);
	a = !!(mesh->format & VERTEX_FORMAT_ALPHA);
	stride = 3 + uv*2 + rgb*3 + a;
	index = 0;

	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)0);
	glEnableVertexAttribArray(index);
	index++;
	if (uv) {
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(index);
		index++;
	}
	if (rgb) {
		glVertexAttribPointer(index, 3+a, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)((3+uv*2)*sizeof(float)));
		glEnableVertexAttribArray(index);
		index++;
	}
}

GFXmesh gfxGenerateTri(float b, float h) {
	GFXmesh mesh = gfxMeshStart(VERTEX_FORMAT_UV);
	float vert[3*5] = {
		-b/2,  -h/2, 0.0f,  0.0f, 1.0f,
    	 b/2,  -h/2, 0.0f,  1.0f, 0.0f,
    	 0.0f,  h/2, 0.0f, 	0.5f, 1.0f};
   	unsigned int indcs[3] = {0,1,2};
	gfxMeshAddVertices(&mesh, GL_STATIC_DRAW, vert, 3, indcs, 3);
	gfxMeshFinish(&mesh);
	return mesh;
}

GFXmesh gfxGenerateRect(float w, float h) {
	GFXmesh mesh = gfxMeshStart(VERTEX_FORMAT_UV);
	float vert[4*5] = {
		-w/2,	 h/2, 0.0f, 0.0f, 0.0f,
		 w/2,	 h/2, 0.0f, 1.0f, 0.0f,
		 w/2,	-h/2, 0.0f, 1.0f, 1.0f,
		-w/2,	-h/2, 0.0f, 0.0f, 1.0f};
	unsigned int indcs[6] = {3,0,1,1,2,3};
	gfxMeshAddVertices(&mesh, GL_STATIC_DRAW, vert, 4, indcs, 6);
	gfxMeshFinish(&mesh);
	return mesh;
}

GFXtexture gfxLoadTexture(char* filepath, GLint format) {

	GFXtexture tex;
	glGenTextures(1, &tex.texture);
	glBindTexture(GL_TEXTURE_2D, tex.texture);
 
 	// CONSIDER stbi_load_from_callbacks(); !!!!!!!!!!!!!!!!
	unsigned char* img_data = stbi_load(filepath, &tex.width, &tex.height, &tex.n_channels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, format, tex.width, tex.height, 0, format, GL_UNSIGNED_BYTE, img_data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// MAKE THIS DEFAULT MORE CUSTOMIZABLE IN FUTURE!!!!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for scaling up

	stbi_image_free(img_data);
	return tex;
}

void gfxUnloadTexture(GFXtexture* texture) {
	glDeleteTextures(1, &texture->texture);
}

void gfxSetShader(GFXshader* shader) {
	ACTIVE_SHADER = shader;
	glUseProgram(shader->program);
}

GFXshader* gfxGetShader() {
	return ACTIVE_SHADER;
}

GFXshader gfxQuickCreateShader(char* vshader_fp, char* fshader_fp) {
	GFXsfrag frags[2] = {gfxBuildShaderFragment(vshader_fp, GL_VERTEX_SHADER), gfxBuildShaderFragment(fshader_fp, GL_FRAGMENT_SHADER)};
	return gfxCreateShader(frags, 2);
}

GFXshader gfxCreateShader(GFXsfrag* fragments, int n_frags) {
	char info_log[512];
	int success;
	GFXshader shd = {glCreateProgram()};

	for (int o=0;o<n_frags;o++) {
		glAttachShader(shd.program, fragments[o]);}
	glLinkProgram(shd.program);
	glGetProgramiv(shd.program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shd.program, 512, NULL, info_log);
		printf("FAILED TO LINK SHADER : %s\n", info_log);
	}

	for (int o=0;o<n_frags;o++) {
		glDeleteShader(fragments[o]);} // Theres probably a way to do this earlier

	return shd;
}

GFXsfrag gfxBuildShaderFragment(char* filepath, GLenum shader_type) {
	GLchar* shader_source;
	char info_log[512];
	int success;

	// Load the shader into a buffer
	shader_source = fileGetString(filepath);

	// Create and compile the shader
	unsigned int shader = glCreateShader(shader_type);

	glShaderSource(shader, 1, (const char**)&shader_source, NULL);
	glCompileShader(shader);

	// Free the buffer (I feel like making this into its own buffer is a waste, no way to have the shader sourced directly from the file?)
	free(shader_source);

	// Check if the process worked
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		printf("%s : FAILED TO COMPILE : %s\n", filepath, info_log);
		return -1;
	}

	return shader;
}

void gfxShaderSetUniformVec2(GFXshader* shader, char* name, float x, float y) {
	int unloc = glGetUniformLocation(shader->program, name);
	glUniform2f(unloc, x, y);
}

void gfxShaderSetUniformMat4(GFXshader* shader, char* name, mat4 matrix) {
	int unloc = glGetUniformLocation(shader->program, name);
	glUniformMatrix4fv(unloc, 1, GL_FALSE, (const float*)matrix); // Does this need to be const?
}

void _DEFAULT_WINDOW_CLOSE_CALLBACK(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, 1);
}

void _DEFAULT_FRAMEBUFFER_CALLBACK(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); // Directly scales window when resized :3
}
