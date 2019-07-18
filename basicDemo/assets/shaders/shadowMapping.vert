#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec2 vertexTextureCoord;
// Atributte 2 of the vertex
layout (location = 2) in vec3 vertexNormal;
// Atributte 3 of the vertex
layout (location = 3) in vec3 vertexTangent;
// Atributte 3 of the vertex
layout (location = 4) in vec3 vertexBitangent;  

//Uniforms MVP matrix
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform mat4 lightSpaceMatrix;
//Texture
out vec2 texCoord;
//Model data
out vec3 Normal;
out vec3 fragPos;
out vec4 FragPosLightSpace;

void main()
{
    fragPos = vec3(Model * vec4(vertexPosition, 1.0f));
    FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
    Normal = (mat3(transpose(inverse(Model))) * vertexNormal);

    texCoord = vertexTextureCoord;
    gl_Position = Proj * View * (vec4(fragPos, 1.0f));
}