#version 330 core
// Vertex color (interpolated/fragment)
in vec4 vColor;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(vColor);
}