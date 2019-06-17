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

vec3 intensiyLightDir(vec3 Normal, vec3 Light, vec3 ViewDir)
{
    vec3 R = reflect(-Light, Normal);
    
    //Material with light components
    vec3 ambient  = ka * ambientColor;
    vec3 diffuse  = kd * diffuseColor * texture2D(text, texCoord) * max(0.0, dot(Normal, Light));
    vec3 specular = ks * specularColor * pow(max(0.0, dot(R, ViewDir)), n);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 Normal = normalize(viewNorm);
    vec3 Light = normalize(lightDir);
    vec3 ViewDir = normalize(viewVec);
    vec3 intensity = intensiyLightDir(Normal, Light, ViewDir);
    color = vec4(intensity, 1.0f);
    
    if(!on)
    {
        vec3 ambient  = ka * ambientColor;
        color = vec4(ambient, 1.0f);

    }
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}