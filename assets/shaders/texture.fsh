#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, TexCoord);
    if (FragColor.r <= 0.5f) {
        FragColor.a = 0.0f;
        for (float i=0.0f; i<50.0f; i++) {
            if (texture(tex, TexCoord + vec2(i / 1000.0f,i / 1000.0f)).r >= 0.5) {
                if (((i / 1000.0f) + TexCoord.x >= 1.0f) || ((i / 1000.0f) + TexCoord.y >= 1.0f)) {
                   return;
                }
                FragColor = vec4(0.0f, 0.0f, 1.0f, 10.0f / i);
                return;
            }
            if (texture(tex, TexCoord + vec2(-i / 1000.0f,-i / 1000.0f)).r >= 0.5) {
                if (((i / 1000.0f) - TexCoord.x >= 0.0f) || ((i / 1000.0f) - TexCoord.y >= 0.0f)) {
                   return;
                }
                FragColor = vec4(1.0f, 1.0f, 0.0f, 10.0f / i);
                return;
            }
        }
    }

    //FragColor = vec4(0.18f, 0.53f, 0.84f, 1.0f);
    //FragColor = vec4(TexCoord.x, TexCoord.y, 1.0f, 1.0f);
}  