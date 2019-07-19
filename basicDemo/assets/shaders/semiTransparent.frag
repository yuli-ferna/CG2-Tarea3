#version 330 core
// Vertex color (interpolated/fragment)
in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;

//Texture
uniform vec3 viewPos;
uniform sampler2D normalMap;
uniform sampler2D dispMap;
uniform sampler2D blend;

// Fragment Color
out vec4 color;

void main()
{
     float ratio = 1.00 / 2.42;
    vec3 I = normalize(fragPos - viewPos);
    // vec3 R = refract(I, normalize(Normal), ratio);
    vec3 R = reflect(I, normalize(Normal));
    
    color = texture(blend, texCoord);
}