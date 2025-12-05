#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNormal;

uniform mat4 modelView;
uniform mat4 projection;
uniform float time;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    vec3 displacedPos = aPos;

    // global wobble
    // float wobble = sin(time * 3.0) * 0.1;
    // displacedPos *= (1.0 + wobble);

    // local oscillation
    // displacedPos += aNormal * sin(length(aPos)*4.0 - time*6.0) * 0.05;
	displacedPos += aNormal * sin(aPos.y * 4.0 + time * 6.0) * 1.0;

    // transform
    FragPos = vec3(modelView * vec4(displacedPos, 1.0));
    Normal = mat3(modelView) * aNormal;
    gl_Position = projection * vec4(FragPos, 1.0);
}