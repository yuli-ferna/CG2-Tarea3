#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 positionNormalize;

uniform sampler2D backFaceCubeTexture;

void main()
{
    vec3 depthValue = texture(backFaceCubeTexture, TexCoords).rgb;
	FragColor = vec4(1.0f);
    FragColor = vec4((depthValue), 1.0);
}