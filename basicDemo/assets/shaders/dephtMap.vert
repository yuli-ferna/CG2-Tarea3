#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aText;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 textCoord;

void main()
{
    textCoord = aText;
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}  