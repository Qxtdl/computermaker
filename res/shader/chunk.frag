#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in float highlight;

uniform sampler2D texture1;

void main()
{
    vec4 color = texture(texture1, TexCoord);
    if (highlight == 1)
        color.rgb += vec3(1, 0, 0);
    FragColor = color;
}
