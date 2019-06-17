#version 330 core
// Vertex color (interpolated/fragment)
in vec3 viewNorm;
in vec3 viewVec;
in vec4 vColor;
in vec2 texCoord;

//Texture
uniform sampler2D text;

struct PointLight {    
    vec3 position;
    vec3 attenuationK; // k0 k1 k2
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    bool on;
};    
uniform PointLight pointLights[1];

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

vec3 intensityPointLight(PointLight lightPoint, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPoint.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), n);
    // attenuation
    float distance    = length(lightPoint.position - fragPos);
    float attenuation = 1.0 / (lightPoint.attenuationK.x + lightPoint.attenuationK.y * distance + 
  			     lightPoint.attenuationK.z * (distance * distance));    
     attenuation = 1.0 / (5 + 5 * distance + 
  			     5 * (distance * distance));    

    // combine results
    vec3 ambient  = lightPoint.ambientColor ;
    vec3 diffuse  = vec3(0.75f, 0.0f, 0.0f)  * diff;
    // vec3 diffuse  = lightPoint.diffuseColor  * diff * vec3(texture(diffuseColor, TexCoords));
    vec3 specular = lightPoint.specularColor * spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{
    vec3 Normal = normalize(viewNorm);
    vec3 Light = normalize(lightDir);
    vec3 ViewDir = normalize(viewVec);
    //Calculando intensidad de luz direccional
    vec3 intensity = intensiyLightDir(Normal, Light, ViewDir);
    intensity += intensityPointLight(pointLights[0], Normal, -viewVec, ViewDir);
    color = vec4(intensity, 1.0f);
    

    //Apagada luz direccional
    if(!on)
    {
        vec3 ambient  = ka * ambientColor;
        color = vec4(ambient, 1.0f);

    }
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}