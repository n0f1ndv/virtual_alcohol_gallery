#version 330 core

// TODO:
// * Review this code (TOP PRIORITY)

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTex;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 vNormal;
out vec2 vTex;
out vec3 vWorldPos;

void main() {
    vec4 world = M * vec4(aPos, 1.0);
    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(M))) * aNormal;
    vTex = aTex;
    gl_Position = P * V * world;
}