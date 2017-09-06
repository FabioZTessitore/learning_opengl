#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;

uniform float delta;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(position.x + delta, position.y, position.z, 1.0f);
    vertexColor = aColor;
}
