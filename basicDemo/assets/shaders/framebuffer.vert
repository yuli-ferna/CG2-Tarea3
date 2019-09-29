  
#version 330 core
layout (location = 0) in vec3 aPos;

//Uniforms MVP matrix
//uniform mat4 Model;
//uniform mat4 View;
//uniform mat4 Proj;
out vec2 TexCoords;

void main()
{
    gl_Position = /*Proj * View * Model */ vec4(aPos, 1.0);
	TexCoords = (aPos.xy + vec2(1.0f))/2.0f;
}