#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelM;

void main()
{
    gl_Position = modelM * vec4 (aPos, 1.0f);

}