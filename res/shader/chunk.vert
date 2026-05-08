#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in ivec3 blockinfo;

out vec2 TexCoord;
flat out int highlight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform ivec3 target;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;

    highlight = all(equal(blockinfo, target)) ? 1 : 0;
}