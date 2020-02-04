#version 330 core

out vec4 IlluminatedFrag;

struct OmniLight
{
    vec4 pos; // xyz -> pos ; w -> far clipping plane
    vec4 diffuse;
    vec4 specular;
};

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragUV;

uniform sampler2D diffusemap;
uniform sampler2D specularmap;
uniform samplerCube shadowmap;

layout(std140) uniform GlobalFragData
{
    vec4 EyePos;
};

layout(std140) uniform FragData
{
    OmniLight light;
};

vec4 CalcOmniLight(vec3 normal, vec3 lookDir);
float CalcShadow(vec3 normal);
void main()
{
    // Discard the fragment if the alpha value is lower than 0.1f
    if(texture(diffusemap, FragUV).a < 0.1f)
        discard;
    vec3 normal = normalize(FragNormal);
    vec3 lookDir = normalize(EyePos.xyz - FragPos);
    float shadow = CalcShadow(normal);
    IlluminatedFrag = (1.0f - shadow) * CalcOmniLight(normal, lookDir);
}

vec4 CalcOmniLight(vec3 normal, vec3 lookDir)
{
    vec3 lightDir = normalize(light.pos.xyz - FragPos);
    float diffMod = max(dot(lightDir, normal), 0.0f);
    vec3 halfwayDir = normalize(lightDir + lookDir);
    float specularMod = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);

    vec4 diffuse = light.diffuse * diffMod * texture(diffusemap, FragUV);
    vec4 specular = light.specular * specularMod * texture(specularmap, FragUV);
    return (diffuse + specular);
}

/*
*   Modified from learnopengl.com
*/
// array of offset direction for sampling
#define SAMPLES 20
vec3 GridSamplingDisks[SAMPLES] = vec3[]
(
    vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
    vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcShadow(vec3 normal)
{
    // Get vec between FragPos and lightPos
    vec3 fragToLight = FragPos - light.pos.xyz;

    // Get linear dist from frag to light
    float currentDepth = length(fragToLight);

    float shadow = 0.0f;
    float bias = 0.15f;
    float viewDist = length(EyePos.xyz - FragPos);
    float diskRadius = (1.0f + (viewDist / light.pos.w)) / 25.0f;
    for(int i = 0; i < SAMPLES; ++i)
    {
        float closestDepth = texture(shadowmap, fragToLight + GridSamplingDisks[i] * diskRadius).r;
        closestDepth *= light.pos.w;
        if(currentDepth - bias > closestDepth)
        {
            shadow += 1.0f;
        }
    }
    shadow /= float(SAMPLES);

    return shadow;
}
