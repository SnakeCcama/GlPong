#version 330 core

layout (location = 0) in vec3 bPos;

uniform mat4 paddleMove;
uniform mat4 ballMove;

void main()
{  
    
    gl_Position = ballMove * vec4(bPos, 1.0f);
     
}