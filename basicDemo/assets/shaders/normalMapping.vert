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
uniform mat4 lightSpaceMatrix;

//Point Light
uniform vec3 pointLight[2];

//Directional Light
uniform vec3 lightDir;

//Camera
uniform vec3 viewPos;
//Texture
//Model data
//Shadows
out OUT_ {
    vec2 texCoord;
    vec4 FragPosLightSpace;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 fragPos;
    mat3 TBN;
    vec3 viewPosTang;
    vec3 pointLightPosTang[2];
    vec3 spotLightPosTang;
    vec3 dirLightPosTang;
    vec3 lightDirTang;

}out_;

void main()
{
    out_.fragPos = vec3(Model * vec4(vertexPosition, 1.0f));
    out_.FragPosLightSpace = lightSpaceMatrix * vec4( out_.fragPos, 1.0);
    
    out_.Tangent = (mat3(transpose(inverse(Model))) * vertexTangent);
    out_.Bitangent = (mat3(transpose(inverse(Model))) * vertexBitangent);
    out_.Normal = (mat3(transpose(inverse(Model))) * vertexNormal);

    //Armando la TBN
    out_.TBN = transpose(mat3(out_.Tangent, out_.Bitangent, out_.Normal));
    out_.lightDirTang = out_.TBN * lightDir;
    out_.pointLightPosTang[0] = out_.TBN * pointLight[0];
    out_.pointLightPosTang[1] = out_.TBN * pointLight[1];
    out_.viewPosTang = out_.TBN * viewPos;
    out_.texCoord = vertexTextureCoord;
    out_.fragPos = out_.fragPos;
    gl_Position = Proj * View * (vec4( out_.fragPos, 1.0f));
}