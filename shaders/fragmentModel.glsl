#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

in vec3 vNormal;
in vec2 vTex;
in vec3 vWorldPos;

out vec4 FragColor; 

uniform sampler2D uTexture;
uniform vec3 uColor;
uniform vec3 uViewPos;

#define MAX_LIGHTS 10
uniform PointLight uLights[MAX_LIGHTS];
uniform int uNumLights;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3 ambient = 0.35 * light.color * baseColor;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * baseColor;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * light.color;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    return (ambient + diffuse + specular) * attenuation * light.intensity;
}

void main() {
    vec3 base = texture(uTexture, vTex).rgb;
    if (length(base) < 0.001) base = uColor;

    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(uViewPos - vWorldPos);

    vec3 result = vec3(0.0);

    for(int i = 0; i < uNumLights; i++) {
        result += CalcPointLight(uLights[i], norm, vWorldPos, viewDir, base);
    }

    FragColor = vec4(result, 1.0);
}
