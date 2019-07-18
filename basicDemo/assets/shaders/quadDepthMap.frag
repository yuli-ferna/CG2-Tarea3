#version 330 core

in vec2 textCoord;
uniform sampler2D depthMap;
out vec4 color;

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * 1.0f * 80.5f) / (80.5f + 1.0f - z * (80.5f - 1.0f));	
}

void main()
{             
    float depthValue = texture(depthMap, textCoord).r;
    color = vec4(vec3(depthValue), 1.0); // orthographic
    color = vec4(vec3(LinearizeDepth(depthValue) / 80.5f), 1.0); // perspective
}