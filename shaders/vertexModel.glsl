#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTex;

uniform float vTime;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 vNormal;
out vec2 vTex;
out vec3 vWorldPos;

void main() {
    vec4 world = M * vec4(aPos, 1.0);

    float strength = 0.5;
    float speed = 1.0;
        
    world.x += sin(vTime * speed + world.y) * strength;
    world.z += cos(vTime * speed * 0.8 + world.y) * strength;

    vWorldPos = world.xyz;
    vNormal = mat3(transpose(inverse(M))) * aNormal;
    vTex = aTex;

    gl_Position = P * V * world;
}
