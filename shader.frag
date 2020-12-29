#version 430 core
#extension GL_ARB_bindless_texture : enable
layout(bindless_sampler) uniform;
layout(bindless_image) uniform;

#define AMBIENT 0.2
#define DIFFUSE 0.5
#define SPECULAR 1.0
#define CONSTANT 1.0

out vec4 color;

in VStoFS {
    vec3 pos; //view space
    vec3 rawNormal;
    vec2 uv;
} frag;

struct Light {
	int type; // 0 = off, 1 = directional, 2 = point, 3 = spot
	vec3 color; // 123
    vec3 dir; // 13
    vec3 pos; // 23
	float linear; // 23
	float quadratic; //23
	float innerCutoff; // 3
	float outerCutoff; // 3
};

layout (std430, binding = 0) buffer Lights {
    uint nLights;
    Light lights[];
};

layout (std140, binding = 0) uniform Material {
    float shine;
    sampler2D diffuse;
    sampler2D specular;
} mat;

vec4 calcColor(Light light) {
    vec4 color = vec4(0);
    if (light.type >= 1) {
        vec3 normal = normalize(frag.rawNormal);
        vec3 lightDir = normalize(light.type == 1 ? -light.dir : light.pos - frag.pos);
        float diffPower = max(dot(normal, lightDir), 0);
        vec3 reflection = reflect(-lightDir, normal);
        float specPower = pow(max(dot(normalize(-frag.pos), reflection), 0), mat.shine);
        vec4 ambient = AMBIENT * vec4(light.color, 1) * texture(mat.diffuse, frag.uv);
        vec4 diffuse = DIFFUSE * vec4(light.color, 1) * texture(mat.diffuse, frag.uv) * diffPower;
        vec4 specular = SPECULAR * vec4(light.color, 1) * texture(mat.specular, frag.uv) * specPower;
        color = ambient + diffuse + specular;

        if (light.type >= 2) {
            float dist = length(light.pos - frag.pos);
            float attenuation = CONSTANT + light.linear * dist + light.quadratic * (dist * dist);
            float luminosity = 1.0 / attenuation;
            color *= luminosity;

            if (light.type >= 3) {
                float theta = dot(-lightDir, light.dir);
                float epsilon = light.innerCutoff - light.outerCutoff;
                float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); 
                color *= intensity;
            }
        }
    }
    return color;
}

void main()
{
    color = vec4(0, 0, 0, 1);
    for (int i = 0; i < nLights; i++)
        color += calcColor(lights[i]);
} 