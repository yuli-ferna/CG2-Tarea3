#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;

//Uniforms MVP matrix
uniform mat4 View;
uniform mat4 Proj;

out vec3 texCoord;

void main()
{
    texCoord = vertexPosition;
    vec4 pos = Proj * View * vec4(vertexPosition, 1.0);
    gl_Position = pos.xyww;
}