#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat4 modelMat; // model matrix
uniform mat4 viewMat;  // view matrix
uniform mat4 projMat;  // projection matrix

out vec3 FragPos;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
}
