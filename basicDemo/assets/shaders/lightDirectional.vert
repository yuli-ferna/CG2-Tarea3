#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec2 vertexTextureCoord;
// Atributte 2 of the vertex
layout (location = 2) in vec3 vertexNormal;

//Uniforms MVP matrix
uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

//Light direction
uniform vec3 lightDir;
uniform vec3 lightPos;

uniform vec3 viewPos;

out vec4 vColor;
out vec2 texCoord;

out vec3 normal;
out vec3 viewVec;
out vec3 fragPos;

void main()
{

    vec4 fragPos = /*View **/ Model * vec4(vertexPosition, 1.0f);

    normal = mat3(transpose(inverse(Model))) * vertexNormal;
    
    viewVec = (viewPos - fragPos.xyz);
 
    texCoord = vertexTextureCoord;
    gl_Position = MVP * (vec4(vertexPosition, 1.0f));
}