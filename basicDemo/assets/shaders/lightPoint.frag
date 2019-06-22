#version 330 core
// Vertex color (interpolated/fragment)
in vec3 Normal;
in vec3 viewVec;
in vec3 fragPos;
in vec4 vColor;
in vec2 texCoord;

//Texture
uniform sampler2D text;

//Camera
uniform vec3 viewPos;

//Light
uniform vec3 lightDir;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform bool on;

//Point Light
struct PointLight {    
    vec3 position;
    vec3 attenuationK; // k0 k1 k2
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    bool on;
};    
uniform PointLight pointLights[2];


//Material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float n;

// Fragment Color
out vec4 color;

vec3 intensityPointLight(PointLight pointLight, vec3 normal, vec3 ViewDir)
{

    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 R = reflect(-lightDir, normal);
    
    vec3 ambient  = ka * pointLight.ambientColor;
    if(!pointLight.on)
        return ambient;
    vec3 diffuse  = kd * pointLight.diffuseColor * texture2D(text, texCoord).rgb * max(0.0, dot(normal, lightDir));
    vec3 specular = ks * pointLight.specularColor * pow(max(0.0, dot(R, ViewDir)), n);
    
    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.attenuationK.x 
        + pointLight.attenuationK.y * dist
        + pointLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;  
    
    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 ViewDir = normalize(viewPos - fragPos);
    
    vec3 result = intensityPointLight(pointLights[0], normal, ViewDir);
    result += intensityPointLight(pointLights[1], normal, ViewDir);
    
    color = vec4(result, 1.0f);
        
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}