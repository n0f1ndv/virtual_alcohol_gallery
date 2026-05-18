#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;
uniform int drunk;

// TODO: Fix this shader, quickly!
vec4 blink(float strength, vec3 color) {
    float coeff = (sin(strength * time) + 1) / 2; 

    return vec4(vec3(coeff * color), 1.0);
}

vec4 negative(vec3 color) {
    return vec4((vec3(1.0) - color), 1.0);
}

vec4 grayscale(vec3 color) {
    float avg = (color.r + color.g + color.b) / 3;

    return vec4(avg, avg, avg, 1.0);
}

vec4 fluctuateColors(vec3 color) {
    return vec4(sin(color.b * time), 0.0, cos(color.r * time), 1.0);
}

float fractal(vec2 uv, vec3 sceneColor) {
    vec2 c = (uv - 0.5) * 2.0; 
    
    vec2 z = c; 
    vec2 juliaConstant = vec2(sin(time * 0.5) * 0.4, cos(time * 0.3) * 0.4);
    
    int iterations = 0;
    const int maxIterations = 32;

    for (int i = 0; i < maxIterations; i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + juliaConstant;

        if (length(z) > 3.0) break;
        iterations++;
    }
    
    return float(iterations) / float(maxIterations);
}

void main()
{ 
    vec2 centre = vec2(0.5);
    vec2 dir = centre - TexCoords;
    
    float d = length(dir);
    dir = normalize(dir);

    float e = 2.718;
    float func = pow(log(time), sin(time)) * e;
    d *= 1. - smoothstep(0.0, func, abs(d));

    vec3 color = texture(screenTexture, TexCoords).rgb; //  TexCoords + dir * d

    // If you want to be high af uncomment line below:
    if (drunk == 1) {
       color = texture(screenTexture, TexCoords + fractal(TexCoords, color) + dir * d).rgb;
    }

    gl_FragColor = vec4(color, 1.0);
}