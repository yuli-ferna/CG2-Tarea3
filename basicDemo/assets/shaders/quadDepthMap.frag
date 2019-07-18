#version 330 core

in vec2 textCoord;
uniform sampler2D depthMap;
out vec4 color;
void main()
{             
    color = vec4(vec3(texture(depthMap, textCoord).r), 1.0f);
} 