#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D backFaceCubeTexture;

void main()
{             //arreglar vertx y fragment y llamar bien al renderquad ese si tienen las coordenadas de textura
    vec3 depthValue = texture(backFaceCubeTexture, TexCoords).rgb;
	FragColor = vec4(1.0f);
    FragColor = vec4((depthValue), 1.0);
}  