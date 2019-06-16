#version 330 core
// Vertex color (interpolated/fragment)
in vec3 N1;
in vec3 L1;
in vec3 V1;
in vec4 vColor;
in vec2 texCoord;

uniform sampler2D text;

uniform vec3 specular_color = vec3(0.60, 0.20, 0.10);
uniform vec3 diffuse_color = vec3(0.30, 0.30, 0.30);
uniform vec3 ambient_color = vec3(0.1);

uniform float ka = 0.10;
uniform float kd = 0.55;
uniform float ks = 0.70;
uniform float n = 32;
// Fragment Color
out vec4 color;

void main()
{
    // Normalize the incoming N, L, and V vectors
    vec3 N = normalize(N1);
    vec3 L = normalize(L1);
    vec3 V = normalize(V1);
     // Calculate R by reflecting -L around the plane defined by N
    vec3 R = reflect(-L, N);
    
            // Calculate ambient, difusse, specular contribution
    vec3 ambient  = ka * ambient_color;
    vec3 diffuse  = kd * diffuse_color * max(0.0, dot(N, L));
    vec3 specular = ks * specular_color * pow(max(0.0, dot(R, V)), n);
    color = vec4(ambient + diffuse + specular, 1.0f);
   
    color = color * texture2D(text, texCoord);
    // color = vColor;
}