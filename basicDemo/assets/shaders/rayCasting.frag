#version 330 core
out vec4 FragColor;
  
in vec3 posNormalized;

uniform sampler3D volumeText;
uniform sampler2D backFaceCubeTexture;
uniform vec2 winSize;


void main()
{       
	vec4 fragEnd = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec2 textCoord = (gl_FragCoord.xy)/winSize.xy;
	vec3 rayDirection = vec3(texture(backFaceCubeTexture,textCoord).xyz - posNormalized);
	vec3 rayI = posNormalized;
	float size = length(rayDirection);
	rayDirection = normalize(rayDirection);
	float step = 1.0f / 256.0f;
	vec3 stepMove;

	for(float i=0.0f; i < size; i += step)
	{
		stepMove = texture(volumeText,rayI).r * vec3(texture(volumeText,rayI).r);
		stepMove *= fragEnd.a;
		fragEnd.rgb += stepMove;
		fragEnd.a *= 1 - texture(volumeText,rayI).r;
		if(1 - fragEnd.a >= 0.99f) break;
		rayI += rayDirection*step;
	}
	fragEnd.a = 1.0f;
	FragColor = fragEnd;
}  