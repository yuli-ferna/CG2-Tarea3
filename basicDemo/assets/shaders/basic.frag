#version 330 core
// Vertex color (interpolated/fragment)
in vec4 vColor;
in vec2 texCoord;

uniform sampler2D text;

// Fragment Color
out vec4 color;

void main()
{
    color = texture2D(text, texCoord);
    // color = vColor;
}