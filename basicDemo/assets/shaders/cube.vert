  
#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aCoordText;

//Uniforms MVP matrix
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec3 ourColor;
//out vec2 textCoord;

void main()
{
    gl_Position = Proj * View * Model * vec4(aPos, 1.0);
 //   ourColor = vec3(aCoordText, 1.0f);
	ourColor = aPos;
	//textCoord = aCoordText;
}