#version 330 core

in vec3 vNormal;
in vec2 vTex;
in vec3 vWorldPos;

uniform float fTime;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;
uniform vec3 uColor;

out vec4 FragColor;

void main() {
    vec3 base = texture(uTexture, vTex).rgb;

    if(length(base) < 0.001) base = uColor;
    
    FragColor = vec4(base, 1.0);
}
