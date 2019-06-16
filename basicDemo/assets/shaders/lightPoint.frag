#version 330 core
// Vertex color (interpolated/fragment)
in vec3 viewNorm;
in vec3 viewVec;
in vec4 vColor;
in vec2 texCoord;

//Texture
uniform sampler2D text;

//Light
uniform vec3 lightDir;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform bool on;

//Material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float n;
uniform float n1 = 50;
// Fragment Color
out vec4 color;

void main()
{
    vec3 N = normalize(viewNorm);
    vec3 L = normalize(lightDir);
    vec3 V = normalize(viewVec);

    vec3 R = reflect(-L, N);
    
    //Material with light components
    vec3 ambient  = ka * ambientColor;
    vec3 diffuse  = kd * texture2D(text, texCoord) * max(0.0, dot(N, L));
    vec3 specular = ks * specularColor * pow(max(0.0, dot(R, V)), n);
    color = vec4(ambient + diffuse + specular, 1.0f);
    if(!on)
        color = vec4(ambient, 1.0f);
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}