#version 330 core

layout (location = 0) in vec3 aPos;  // input vertex in local coordinates
layout (location = 2) in vec2 aTexCoord;  // texture coordinates
layout (location = 3) in vec3 aNormal;  // input vertex in local coordinates

uniform mat4 modelMat; // model matrix
uniform mat4 viewMat;  // view matrix
uniform mat4 projMat;  // projection matrix

out vec3 FragPos;  // for the fragment shader
out vec3 Normal;
out vec2 texCoord;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(aPos, 1.0);
	FragPos = vec3(viewMat * modelMat * vec4(aPos, 1.0));
	Normal = (viewMat * modelMat * vec4(aNormal, 0.0)).xyz; // we assume uniform scale
    
    texCoord = aTexCoord;
}