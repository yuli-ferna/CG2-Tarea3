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
uniform float n; // Shinniness
uniform float roughness;
uniform bool albedo;
uniform float f0 = 0.8;

// Fragment Color
out vec4 color;

float calculateCookTorrence(float NdotL, vec3 normal, vec3 lightDir, vec3 ViewDir)
{
    float Rs;
    vec3 HalfwayDir = normalize(lightDir + ViewDir);
    float NdotH = clamp(dot(normal, HalfwayDir), 0.0f, 1.0f);
    float NdotV = clamp(dot(normal, ViewDir), 0.0f, 1.0f);
    float VdotH = clamp(dot(lightDir, HalfwayDir), 0.0f, 1.0f);

    // Fresnel (usamos shinnines como material de reflectancia)
    float fresnel = (pow(1.0 - VdotH, 5.0) * (1.0 - f0)) + f0;

    // Microfacet distribution by Beckmann
    float m2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;
    float PI = 3.14159265359;
    float r1 = 1.0 / ( PI * m2 * pow(NdotH, 4.0));
    float r2 = (NdotH2 - 1.0) / (m2 * NdotH2);
    float Rough = r1  * exp(r2);

    // Geometric shadowing
    float g1 = (2.0 * NdotH * NdotV) / VdotH;
    float g2 = (2.0 * NdotH * NdotL) / VdotH;
    float geometric = min(1.0, min(g1, g2));

    Rs = (fresnel * Rough * geometric) / (  NdotL * NdotV);
	return Rs; 
}

vec3 intensiyLightDir(vec3 Normal, vec3 ViewDir, vec3 diffuseColorK)
{
    vec3 LightDir = normalize(-lightDir); // Solo usamos la entrada del tweakbar
    vec3 reflectDir = reflect(-LightDir, Normal);
    //cooktorrence     
    float NdotL = clamp(dot(Normal, LightDir), 0.0f, 1.0f);
	float Rs = 0.0f;
    if (NdotL > 0) 
	{
        Rs = calculateCookTorrence(NdotL, Normal, LightDir, ViewDir);
	}
    
    //Material with lightDir components
    vec3 ambient  = ka * ambientColor;
    if(!on)
        return ambient;
    vec3 diffuse  = diffuseColorK *  diffuseColor
    * NdotL;
    vec3 specular = ks * specularColor 
    * NdotL * Rs;
    // * NdotL * (f0 + Rs * (1.0 - f0));


    return ambient + diffuse + specular;
}

vec3 intensityPointLight(PointLight pointLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK)
{

    vec3 lightDir = normalize(pointLight.position - fragPos);
    vec3 R = reflect(-lightDir, normal);
    //cooktorrence     
    float NdotL = clamp(dot(normal, lightDir), 0.0f, 1.0f);
	float Rs = 0.0f;
    if (NdotL > 0) 
	{
        Rs = calculateCookTorrence(NdotL, normal, lightDir, ViewDir);
	}

    //cooktorrence     

    vec3 ambient  = ka * pointLight.ambientColor;
    if(!pointLight.on)
        return ambient;
    
    vec3 diffuse  = diffuseColorK * pointLight.diffuseColor
    * NdotL;
    vec3 specular = ks * pointLight.specularColor 
    * NdotL * Rs;
    // * NdotL * (f0 + Rs * (1.0 - f0));

    
    float dist = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.attenuationK.x 
        + pointLight.attenuationK.y * dist
        + pointLight.attenuationK.z * (dist * dist));
    
    // ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;  
    
    return  ambient + diffuse + specular;
}


vec3 intensitySpotLight(spotLight SpotLight, vec3 normal, vec3 ViewDir, vec3 diffuseColorK)
{
    //Spot
    vec3 lightDir = normalize(SpotLight.position - fragPos);
    float theta = dot(lightDir, normalize(-SpotLight.direction));
    float epsilon   = SpotLight.cuttof - SpotLight.outerCuttof;
    //cooktorrence     
    float NdotL = clamp(dot(normal, lightDir), 0.0f, 1.0f);
	float Rs = 0.0f;
    if (NdotL > 0) 
	{
        Rs = calculateCookTorrence(NdotL, normal, lightDir, ViewDir);
	}

    float intensity;
    intensity = (clamp((theta - SpotLight.outerCuttof) / epsilon, 0.0, 1.0));    
    
    vec3 R = reflect(-lightDir, normal);

    vec3 ambient  = ka * SpotLight.ambientColor;
    if(!SpotLight.on)
        return ambient;
    vec3 diffuse  = diffuseColorK * SpotLight.diffuseColor
    * NdotL;
    
    vec3 specular = ks * SpotLight.specularColor 
    * NdotL * Rs;
    // * NdotL * (f0 + Rs * (1.0 - f0));

    
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
    vec3 result = vec3(0.0f);
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