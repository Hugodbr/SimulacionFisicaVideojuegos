#version 330 core

layout (location = 0) in vec3 aPos;      // Vertex position
// layout (location = 1) in vec3 aColor;    // Vertex color

// out vec3 vertexColor;                    // Pass color to fragment shader

uniform mat4 modelMat; // model matrix
uniform mat4 viewMat;  // view matrix
uniform mat4 projMat;  // projection matrix

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
    // vertexColor = aColor;
}