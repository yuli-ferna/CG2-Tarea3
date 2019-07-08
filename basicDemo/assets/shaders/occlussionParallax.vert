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
// uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
//Camera
uniform vec3 viewPos;
//Light
uniform vec3 lightDir;

//Texture
out vec2 texCoord;
//Model data
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 fragPos;
out mat3 TBN;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main()
{
    fragPos = vec3(Model * vec4(vertexPosition, 1.0f));
    
    Tangent = (mat3(transpose(inverse(Model))) * vertexTangent);
    Bitangent = (mat3(transpose(inverse(Model))) * vertexBitangent);
    Normal = (mat3(transpose(inverse(Model))) * vertexNormal);
    //Aplicando grandsmith
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    Bitangent = cross(Normal, Tangent);
    //Armando la TBN
    TBN = transpose(mat3(Tangent, Bitangent, Normal));

    // Calculate tangent
    TangentLightPos = TBN * lightDir;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * fragPos;

    texCoord = vertexTextureCoord;
    gl_Position = Proj * View * (vec4(fragPos, 1.0f));
}