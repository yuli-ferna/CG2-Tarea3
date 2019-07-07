#version 330 core
// Vertex color (interpolated/fragment)
in vec3 texCoord;

//Texture
uniform samplerCube skybox;
// Fragment Color
out vec4 color;

void main()
{
    //Texture
    color = texture(skybox, texCoord);
}