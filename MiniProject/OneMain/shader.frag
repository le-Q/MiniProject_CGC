#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D shadowMap;
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct Light {
    //vec3 position;
    vec4 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform vec3 lightPos;

float shadowCalc(vec4 fragPosLightSpace)
{
    // Perspective division
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Depth value from light
    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 

    // Depth of current fragment
    float currentDepth = projCoords.z;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(material.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }
    }
    shadow /= 9.0;
    // Check if current frag pos is in shadow
    // float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    // Keep shadow at 1 outside of far plane
    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }

    return shadow;
}

void main()
{

	// Ambient
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoord));

    // Diffuse 
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoord)).rgb;  
   
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.5), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoord));  

    // Shadow
    float shadow = shadowCalc(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));
    
    FragColor = vec4(lighting, 1.0);
    //FragColor = vec4(ambient + diffuse + specular, 1.0);   
}