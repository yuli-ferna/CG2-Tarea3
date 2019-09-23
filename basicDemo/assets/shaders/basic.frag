#version 330 core
in vec3 ourColor;
in vec2 textCoord;

//Texture
uniform sampler2D volumeText;

out vec4 FragColor;

void main()
{
    FragColor = vec4(ourColor,1.0f);
//	FragColor = texture3D(volumeText, textCoord);
}