#version 330 core

out vec4 FragColor;

// -----------------------------------------------------------------------------
// Light direction already in view space
uniform vec3 lightDirView;

// -----------------------------------------------------------------------------
// Main shader function
void main()
{
    FragColor = vec4(0.9, 0.9, 1.0, 0.2);
}
