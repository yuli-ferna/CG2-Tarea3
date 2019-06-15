#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec2 vertexTextureCoord;
// Atributte 2 of the vertex
layout (location = 2) in vec3 vertexNormal;

//Uniforms MVP matrix
uniform mat4 MVP;


// Vertex data out data
out vec4 vColor;
out vec2 texCoord;

void main()
{
    vColor =  vec4(vertexNormal, 1.0f);   
    texCoord = vertexTextureCoord;
    gl_Position = MVP * (vec4(vertexPosition, 1.0f));
}