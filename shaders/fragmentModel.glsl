#version 330 core

in vec3 vNormal;
in vec2 vTex;
in vec3 vWorldPos;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;
uniform vec3 uColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightPos - vWorldPos);
    float diff = max(dot(N,L), 0.0);

    vec3 base = texture(uTexture, vTex).rgb;
    if (length(base) < 0.001) base = uColor;

    vec3 ambient = 0.3 * base;

    vec3 diffuse = diff * base;

    vec3 V = normalize(uViewPos - vWorldPos);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), 8.0);
    vec3 specular = vec3(0.15) * spec;
    vec3 color = ambient + diffuse + specular;

    gl_FragColor = vec4(color, 1.0);
}
