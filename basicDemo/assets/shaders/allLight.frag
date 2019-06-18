#version 330 core
// Vertex color (interpolated/fragment)
in vec3 viewNorm;
in vec3 viewVec;
in vec4 vColor;
in vec2 texCoord;
in vec3 lightDir1;

//Texture
uniform sampler2D text;

//Directional Light
uniform vec3 lightDir;
uniform vec3 specularColor;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform bool on;
uniform int Mtl;

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

// //Point Light
struct SpotLight {    
    vec3 position;
    vec3 attenuationK; // k0 = att.x k1 = att.y k2= att.z
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float cuttof;
	float outerCuttof;
    bool on;
};    
uniform SpotLight spotLight;


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
    vec3 R = reflect(-Light, Normal );
    //blinn phon
    vec3 halfwayDir = normalize(Light + ViewDir);
    //Material with light components
    vec3 ambient  = ka * ambientColor;
    vec3 diffuse  = kd * diffuseColor * texture2D(text, texCoord) * max(0.0, dot(Normal, Light));
    //blinn phon specular
    vec3 specular;
    if(Mtl != 0){
        specular = ks * specularColor * pow(max(0.0, dot(Normal, halfwayDir)), n);
    }else{
        specular = vec3(0.0f);
    }

    return ambient + diffuse + specular;
}

vec3 intensityPointLight(PointLight lightPoint, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPoint.position - fragPos);
    vec3 R = reflect(-lightDir, normal );
    float diff = max(dot(normal, lightDir), 0.0);
    float distance    = length(lightPoint.position - fragPos);
    float attenuation = 1.0 / (lightPoint.attenuationK.x + lightPoint.attenuationK.y * distance + 
  			     lightPoint.attenuationK.z * (distance * distance));     

    vec3 ambient  = lightPoint.ambientColor ;
    vec3 diffuse  = lightPoint.diffuseColor * diff * texture2D(text, texCoord);
    // vec3 diffuse  = lightPoint.diffuseColor  * diff;
    ambient  *= attenuation;
    vec3 specular;
    float spec;
    if(Mtl != 0)
    {
        spec = pow(max(dot(viewDir, R), 0.0), n);
        specular = lightPoint.specularColor * spec;
        specular *= attenuation;
    }else{
        specular = vec3(0.0f);
    }

    diffuse  *= attenuation;

    return (ambient + diffuse + specular);
} 

// vec3 intensitySpotLight(SpotLight spot, vec3 Normal, vec3 viewDir)
// {
//     
// }

void main()
{
    vec3 Normal = normalize(viewNorm);
    vec3 Light = normalize(lightDir);
    vec3 ViewDir = normalize(viewVec);
    //Calculando intensidad de luz direccional
    vec3 intensity = ka * ambientColor;
    if(on)    
        intensity = intensiyLightDir(Normal, Light, ViewDir);
    if(pointLights[0].on)
        intensity += intensityPointLight(pointLights[0], Normal, -viewVec, ViewDir);
    if(pointLights[1].on)
        intensity += intensityPointLight(pointLights[1], Normal, -viewVec, ViewDir);
    
    color = vec4(intensity, 1.0f);
    
    //Texture
    // color = color * texture2D(text, texCoord);
    // color = vColor;
}