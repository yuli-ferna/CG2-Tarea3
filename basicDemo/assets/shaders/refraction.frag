#version 330 core
// Vertex color (interpolated/fragment)
in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;

//Texture
uniform vec3 viewPos;
uniform samplerCube skybox;
uniform float indexMaterial;
uniform float indexAmbiental;
// Fragment Color
out vec4 color;

void main()
{
     float ratio = indexAmbiental / indexMaterial;
    vec3 I = normalize(fragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    // vec3 R = reflect(I, normalize(Normal));

    color = vec4(texture(skybox, R).rgb, 1.0);
}