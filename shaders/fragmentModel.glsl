#version 330 core

in vec3 vNormal;
in vec2 vTex;
in vec3 vWorldPos;

uniform float time;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;
uniform vec3 uColor;

vec3 grayscale(vec3 color) {
    float avg = (color.r + color.g + color.b) / 3;

    return vec3(avg);
}

void main() {
    vec3 base = texture(uTexture, vTex).rgb;

    if(length(base) < 0.001) base = uColor;

    gl_FragColor = vec4(base, 1.0);
}
