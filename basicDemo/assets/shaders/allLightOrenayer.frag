#version 330 core
// Vertex color (interpolated/fragment)
in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;

//Texture
uniform sampler2D text;
uniform sampler2D sepcMap;

//Camera
uniform vec3 viewPos;

//Directional Light
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

//Spot Light
struct spotLight {    
    vec3 position;
    vec3 direction;
    vec3 attenuationK; // k0 = att.x k1 = att.y k2= att.z
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float cuttof;
	float outerCuttof;
    bool on;
};    
uniform spotLight SpotLight;

//Model Material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float n;
uniform float roughness;
uniform bool albedo;

// Fragment Color
out vec4 color;

float calculateOrenNayar(vec3 Normal, vec3 LightDir, vec3 ViewDir)
{
    float tita = roughness * roughness;
    
    float dotVN = clamp(dot(Normal, ViewDir), 0.0f, 1.0f);
    float dotLN = clamp(dot(Normal, LightDir), 0.0f, 1.0f);
    float alfa = max(acos(dotLN), acos(dotVN));
    float beta = min(acos(dotLN), acos(dotVN));
    
    float A = 1.0f - (0.5f * tita / (tita + 0.57f));
    float B = 0.45f * (tita / (tita + 0.09f));
    vec3 cosR = normalize(ViewDir - Normal * dotVN);
    vec3 cosI = normalize(LightDir - Normal * dotLN);
    float cosRI = clamp(dot(cosI, cosR), 0.0f, 1.0f);
    
    float Rf = A + (max(0.0f, cosRI) * B * sin(alfa) * tan(beta));
    return Rf;
}

vec3 intensiyLightDir(vec3 Normal, vec3 ViewDir, vec3 diffuseColorK)
{
    vec3 LightDir = normalize(-lightDir); // Solo usamos la entrada del tweakbar
    vec3 reflectDir = reflect(-LightDir, Normal);

    //Material with lightDir components 
    vec3 ambient  = ka * ambientColor;
    if(!on)
        return ambient;
    float Rf = calculateOrenNayar(Normal, LightDir, ViewDir);
    vec3 diffuse = diffuseColorK * diffuseColor * max(0.0, dot(Normal, LightDir)) * Rf;

    return ambient + diffuse;
}

vec3 intensityPointLight(PointLight pointLight, vec3 Normal, vec3 ViewDir, vec3 diffuseColorK)
{

    vec3 LightDir = normalize(pointLight.position - fragPos);
    vec3 R = reflect(-LightDir, Normal);
    
    vec3 ambient  = ka * pointLight.ambientColor;
    if(!pointLight.on)
        return ambient;
    
    float Rf = calculateOrenNayar(Normal, LightDir, ViewDir);
    
    vec3 diffuse  = diffuseColorK * pointLight.diffuseColor * max(0.0, dot(Normal, LightDir)) * Rf;
    
    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.attenuationK.x 
        + pointLight.attenuationK.y * dist
        + pointLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    
    return ambient + diffuse;
}


vec3 intensitySpotLight(spotLight SpotLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK)
{
    //Spot
    vec3 lightDir = normalize(SpotLight.position - fragPos);
    float theta = dot(lightDir, normalize(-SpotLight.direction));
    float epsilon   = SpotLight.cuttof - SpotLight.outerCuttof;

    float intensity;
    intensity = (clamp((theta - SpotLight.outerCuttof) / epsilon, 0.0, 1.0));    
    
    vec3 R = reflect(-lightDir, normal);

    vec3 ambient  = ka * SpotLight.ambientColor;
    if(!SpotLight.on)
        return ambient;
    float Rf = calculateOrenNayar(normal, lightDir, ViewDir);
    
    vec3 diffuse  = diffuseColorK * SpotLight.diffuseColor
    * max(0.0, dot(normal, lightDir)) * Rf;
    
    float dist = length(SpotLight.position - fragPos);
    float attenuation = 1.0f / (SpotLight.attenuationK.x 
        + SpotLight.attenuationK.y * dist
        + SpotLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;

    //Bordes suaves
    diffuse  *= intensity;

    vec3 result =ambient + diffuse; 
    return result;
}

void main()
{
    //Datos de vital importancia para todos
    vec3 normal = normalize(Normal);
    vec3 ViewDir = normalize(viewPos - fragPos.xyz);
    
    vec3 result;
    if(!albedo)
    {
        result = intensitySpotLight(SpotLight, normal, ViewDir, kd);
        result += intensityPointLight(pointLights[0], normal, ViewDir, kd);
        result += intensityPointLight(pointLights[1], normal, ViewDir, kd);    
        result += intensiyLightDir(normal,ViewDir, kd);
    
    }else {
        result = intensitySpotLight(SpotLight, normal, ViewDir, texture2D(text, texCoord).rgb);
        result += intensityPointLight(pointLights[0], normal, ViewDir, texture2D(text, texCoord).rgb);
        result += intensityPointLight(pointLights[1], normal, ViewDir, texture2D(text, texCoord).rgb);
        
        result += intensiyLightDir(normal,ViewDir, texture2D(text, texCoord).rgb);
        if(texture2D(text, texCoord).a < 0.1)
            discard;
    
    }
    color = vec4(result, 1.0f);
    
    //Texture
    // color = color /* texture2D(text, texCoord).rgb*/;
    // color = vColor;
}