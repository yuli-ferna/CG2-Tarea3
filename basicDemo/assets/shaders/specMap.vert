#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec2 vertexTextureCoord;
// Atributte 2 of the vertex
layout (location = 2) in vec3 vertexNormal;

//Uniforms MVP matrix
// uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
//Texture
out vec2 texCoord;
//Model data
out vec3 Normal;
out vec3 fragPos;

void main()
{
    fragPos = vec3(Model * vec4(vertexPosition, 1.0f));

    Normal = (mat3(transpose(inverse(Model))) * vertexNormal);

    texCoord = vertexTextureCoord;
    gl_Position = Proj * View * (vec4(fragPos, 1.0f));
}