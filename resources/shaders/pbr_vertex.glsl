#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPosView;
out mat3 TBN;

void main()
{
    // position
    vec3 posWorld = vec3(model * vec4(aPos, 1.0));
    FragPosView = vec3(view * vec4(posWorld, 1.0));
    gl_Position = projection * vec4(FragPosView, 1.0);

    TexCoord = aUV;

    // normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);
}
