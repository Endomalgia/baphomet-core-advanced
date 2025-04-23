#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, TexCoord);
    //FragColor = vec4(0.18f, 0.53f, 0.84f, 1.0f);
    //FragColor = vec4(TexCoord.x, TexCoord.y, 1.0f, 1.0f);
}  