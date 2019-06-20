#version 330 core
// Vertex color (interpolated/fragment)
in vec3 normal;
in vec3 viewVec;
in vec3 fragPos;
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
uniform float n1 = 50;
// Fragment Color
out vec4 color;

vec3 intensiyLightDir(vec3 Normal, vec3 LightDir, vec3 ViewDir)
{
    vec3 reflectDir = reflect(-LightDir, Normal);
    
    //Material with lightDir components
    vec3 ambient  = ka * ambientColor;
    vec3 diffuse  = kd * diffuseColor * /*texture2D(text, texCoord).rgb **/ max(0.0, dot(Normal, LightDir));
    vec3 specular = ks * specularColor * pow(max(0.0, dot(reflectDir, ViewDir)), n);

    return ambient + diffuse + specular;
}

vec3 intensityPointLight(PointLight lightPoint, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPoint.position - fragPos);
    vec3 R = reflect(-lightDir, normal );
    float diff = max(dot(normal, lightDir), 0.0);
    float distance = length(lightPoint.position - fragPos);
    float attenuation = 1.0 / (lightPoint.attenuationK.x + lightPoint.attenuationK.y * distance + 
  			     lightPoint.attenuationK.z * (distance * distance));     

    vec3 ambient  = lightPoint.ambientColor ;
    vec3 diffuse  = lightPoint.diffuseColor * diff /* texture2D(text, texCoord).rgb*/;
    // vec3 diffuse  = lightPoint.diffuseColor  * diff;
    ambient  *= attenuation;
    vec3 specular;
    float spec;
        spec = pow(max(dot(viewDir, R), 0.0), n);
        specular = lightPoint.specularColor * spec;
        specular *= attenuation;
    

    diffuse  *= attenuation;

    return (ambient + diffuse + specular);
} 


void main()
{
    vec3 Normal = normalize(normal);
    vec3 Light = normalize(-lightDir); // Solo usamos la entrada del tweakbar
    vec3 ViewDir = normalize(viewVec);
    vec3 intensity = ka * ambientColor; 
    if(on)
        intensity += intensiyLightDir(Normal, Light, ViewDir);
        intensity += intensityPointLight(pointLights[0], Normal, fragPos, ViewDir);
    color = vec4(intensity, 1.0f);
    
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}