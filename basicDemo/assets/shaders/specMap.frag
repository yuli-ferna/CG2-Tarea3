#version 330 core
// Vertex color (interpolated/fragment)
in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;

//Texture
uniform sampler2D text;
uniform sampler2D specMap;

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
uniform bool albedo;

// Fragment Color
out vec4 color;

vec3 intensiyLightDir(vec3 Normal, vec3 ViewDir, vec3 diffuseColorK, vec3 specularColorK)
{
    vec3 LightDir = normalize(-lightDir); // Solo usamos la entrada del tweakbar
    vec3 reflectDir = reflect(-LightDir, Normal);
    vec3 halfwayDir = normalize(LightDir + ViewDir);
    //Material with lightDir components
    vec3 ambient  = ka * ambientColor;
    if(!on)
        return ambient;
    vec3 diffuse  = diffuseColorK * diffuseColor * max(0.0, dot(Normal, LightDir));
    vec3 specular = specularColorK * specularColor * pow(max(0.0, dot(reflectDir, halfwayDir)), n);

    return ambient + diffuse + specular;
}

vec3 intensityPointLight(PointLight pointLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK, vec3 specularColorK)
{

    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 R = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + ViewDir);
    
    vec3 ambient  = ka * pointLight.ambientColor;
    if(!pointLight.on)
        return ambient;
    vec3 diffuse  = diffuseColorK * pointLight.diffuseColor * max(0.0, dot(normal, lightDir));
    vec3 specular = specularColorK * pointLight.specularColor * pow(max(0.0, dot(R, halfwayDir)), n);
    
    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.attenuationK.x 
        + pointLight.attenuationK.y * dist
        + pointLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;  
    
    return ambient + diffuse + specular;
}


vec3 intensitySpotLight(spotLight SpotLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK, vec3 specularColorK)
{
    //Spot
    vec3 lightDir = normalize(SpotLight.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + ViewDir);

    float theta = dot(lightDir, normalize(-SpotLight.direction));
    float epsilon   = SpotLight.cuttof - SpotLight.outerCuttof;

    float intensity;
    intensity = (clamp((theta - SpotLight.outerCuttof) / epsilon, 0.0, 1.0));    
    
    vec3 R = reflect(-lightDir, normal);

    vec3 ambient  = ka * SpotLight.ambientColor;
    if(!SpotLight.on)
        return ambient;
    vec3 diffuse  = diffuseColorK * SpotLight.diffuseColor * max(0.0, dot(normal, lightDir));
    vec3 specular = specularColorK * SpotLight.specularColor * pow(max(0.0, dot(R, halfwayDir)), n);
    
    float dist = length(SpotLight.position - fragPos);
    float attenuation = 1.0f / (SpotLight.attenuationK.x 
        + SpotLight.attenuationK.y * dist
        + SpotLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;  
    //Bordes suaves
    diffuse  *= intensity;
    specular *= intensity;
    vec3 result =ambient + diffuse + specular; 
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
        result = intensitySpotLight(SpotLight, normal, ViewDir, kd, ks);
        result += intensityPointLight(pointLights[0], normal, ViewDir, kd, ks);
        result += intensityPointLight(pointLights[1], normal, ViewDir, kd, ks);
        result += intensiyLightDir(normal,ViewDir, kd, ks);
    
    }else {
        result = intensitySpotLight(SpotLight, normal, ViewDir, texture2D(text, texCoord).rgb, texture2D(specMap, texCoord).rgb);
        result += intensityPointLight(pointLights[0], normal, ViewDir, texture2D(text, texCoord).rgb, texture2D(specMap, texCoord).rgb);
        result += intensityPointLight(pointLights[1], normal, ViewDir, texture2D(text, texCoord).rgb, texture2D(specMap, texCoord).rgb);
        result += intensiyLightDir(normal,ViewDir, texture2D(text, texCoord).rgb, texture2D(specMap, texCoord).rgb);
        if(texture2D(text, texCoord).a < 0.1)
            discard;
    }
    color = vec4(result, 1.0f);
    
    //Texture
    // color = color /* texture2D(text, texCoord).rgb*/;
    // color = vColor;
}