#version 330 core
// Vertex color (interpolated/fragment)
in vec3 Normal;
in vec3 fragPos;
in vec2 texCoord;
in vec4 FragPosLightSpace;

//Texture
uniform sampler2D text;
uniform sampler2D specMap;
uniform sampler2D shadowMap;

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

float intensityShadow(vec3 Normal, vec3 LightDir, vec4 FragPosLightSpace)
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 
    float closestDepth = texture(shadowMap, projCoords.xy).r;   
    float currentDepth = projCoords.z;  
    float bias = max(0.05 * (1.0 - dot(normalize(Normal), LightDir)), 0.005);  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

vec3 intensiyLightDir(vec3 Normal, vec3 ViewDir, vec3 diffuseColorK)
{
    vec3 LightDir = normalize(-lightDir); // Solo usamos la entrada del tweakbar
    vec3 reflectDir = reflect(-LightDir, Normal);
    vec3 halfwayDir = normalize(LightDir + ViewDir);
    //Material with lightDir components
    vec3 ambient  = ka * ambientColor;
    if(!on)
        return ambient;
    vec3 diffuse  = diffuseColorK * diffuseColor * max(0.0, dot(Normal, LightDir));
    vec3 specular = ks * specularColor * pow(max(0.0, dot(reflectDir, halfwayDir)), n);

    float shadow = intensityShadow(Normal, LightDir, FragPosLightSpace);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 intensityPointLight(PointLight pointLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK)
{

    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 R = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + ViewDir);
    
    vec3 ambient  = ka * pointLight.ambientColor;
    if(!pointLight.on)
        return ambient;
    vec3 diffuse  = diffuseColorK * pointLight.diffuseColor * max(0.0, dot(normal, lightDir));
    vec3 specular = ks * pointLight.specularColor * pow(max(0.0, dot(R, halfwayDir)), n);
    
    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.attenuationK.x 
        + pointLight.attenuationK.y * dist
        + pointLight.attenuationK.z * (dist * dist));
    
    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;  
    
    return ambient + diffuse + specular;
}


vec3 intensitySpotLight(spotLight SpotLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK)
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
    vec3 specular = ks * SpotLight.specularColor * pow(max(0.0, dot(R, halfwayDir)), n);
    
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