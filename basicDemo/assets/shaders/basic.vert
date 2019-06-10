#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec4 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec4 vertexColor;


// Vertex data out data
out vec4 vColor;

void main()
{
    vColor =  vertexColor;   
    gl_Position = (vertexPosition);
}