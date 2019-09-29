  
#version 330 core
layout (location = 0) in vec3 aPos;

//Uniforms MVP matrix
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
out vec3 posNormalized;

void main()
{
    gl_Position = Proj * View * Model * vec4(aPos, 1.0);
	posNormalized = aPos + (0.5f);
}