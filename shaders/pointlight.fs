#version 330 core

out vec4 IlluminatedFrag;

struct PointLight
{
//TODO: Having the mat4 multiplied in every fragment is not efficient at all
    mat4 lpv;
    vec4 pos;
    vec4 diffuse;
    vec4 specular;
};

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragUV;
//in vec4 FragShadow;

uniform sampler2D diffusemap;
uniform sampler2D specularmap;
uniform sampler2D shadowmap;

layout(std140) uniform GlobalFragData
{
    vec4 EyePos;
};

layout(std140) uniform FragData
{
    PointLight pointLight;
};

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 lookDir);
float CalcShadow(vec3 lightPos, vec3 normal);
void main()
{
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texture(diffusemap, FragUV).a < 0.1f)
        discard;
    vec3 normal = normalize(FragNormal);
    vec3 lookDir = normalize(EyePos.xyz - FragPos);
    float shadow = CalcShadow(pointLight.pos.xyz, normal);
    IlluminatedFrag = (1.0f - shadow) * CalcPointLight(pointLight, normal, lookDir);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 lookDir)
{
    vec3 lightDir = normalize(light.pos.xyz - FragPos);
    float diffMod = max(dot(lightDir, normal), 0.0f);
    vec3 halfwayDir = normalize(lightDir + lookDir);
    float specularMod = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);

    vec4 diffuse = light.diffuse * diffMod * texture(diffusemap, FragUV);
    vec4 specular = light.specular * specularMod * texture(specularmap, FragUV);
    return (diffuse + specular);
}

/**
    Modified from learnopengl.com
*/
float CalcShadow(vec3 lightPos, vec3 normal)
{
    //vec3 projCoords = FragShadow.xyz / FragShadow.w;
    vec4 FragShadow = pointLight.lpv * vec4(FragPos, 1.0f);
    vec3 projCoords = FragShadow.xyz / FragShadow.w;
    projCoords = projCoords * 0.5f + 0.5f;

    float closestDepth = texture(shadowmap, projCoords.xy).r;

    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(lightPos - FragPos);

    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);

    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowmap, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowmap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }

    shadow /= 9.0f;

    if(projCoords.z > 1.0f)
    {
        shadow = 0.0f;
    }

    return shadow;
}
