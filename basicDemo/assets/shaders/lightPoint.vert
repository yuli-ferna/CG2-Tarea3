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

uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

out vec4 vColor;
out vec2 texCoord;

out vec3 viewNorm;
out vec3 viewVec;

void main()
{

    vec4 P = View * Model * vec4(vertexPosition, 1.0f);

    viewNorm = (mat3(View * Model) * vertexNormal);
    
    viewVec = (-P.xyz);
 
    texCoord = vertexTextureCoord;
    gl_Position = MVP * (vec4(vertexPosition, 1.0f));
}