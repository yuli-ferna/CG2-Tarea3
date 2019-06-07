#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}