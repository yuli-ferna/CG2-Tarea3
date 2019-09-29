#version 330 core
in vec3 ourColor;
in vec2 textCoord;

//Texture
uniform sampler3D volumeText;
uniform float step;

out vec4 FragColor;

void main()
{
	vec3 text = vec3(textCoord.x, textCoord.y, step);
    FragColor = vec4(1.0f);
	FragColor = vec4(vec3(texture(volumeText,text).r), 1.0f);
}