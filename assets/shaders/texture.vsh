#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 position;
uniform vec3 scale;

uniform mat4 view;
uniform mat4 projec;


void main() {
	gl_Position = projec*view*(vec4(aPos * scale, 1.0f) + vec4(position, 0.0f, 0.0f));
	TexCoord = aTexCoord;
}