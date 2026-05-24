#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;
uniform int drunk;

vec3 blink(float strength, vec3 color) {
    float coeff = (sin(strength * time) + 1) / 2; 

    return vec3(coeff * color);
}

vec3 fluctuateColors(vec3 color) {
    return vec3(0.0, sin(color.b * time), cos(color.r * time));
}

float fractal(vec2 uv, vec3 sceneColor) {
    vec2 c = (uv - 0.5) * 2.0; 
    
    vec2 z = c; 
    vec2 juliaConstant = vec2(sin(time * 0.5) * 0.5, cos(time * 0.5) * 0.5);
    
    int iterations = 0;
    const int maxIterations = 128;

    for (int i = 0; i < maxIterations; i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + juliaConstant;

        if (length(z) > 6.0) break;
        iterations++;
    }
    
    return float(iterations) / float(maxIterations);
}

void main()
{
    vec3 color = texture(screenTexture, TexCoords).rgb;

    if (drunk >= 4) {
       color = texture(screenTexture, TexCoords + fractal(TexCoords, color)).rgb; 
    }
    if (drunk >= 1) {
        vec2 centre = vec2((sin(time) + 1) / 2);
        vec2 dir = centre - TexCoords;
        
        float d = length(dir);
        dir = normalize(dir);
    
        float e = 2.718;
        float func = 3 * abs(cos(0.5 * time));
        d *= 1. - smoothstep(0.0, func, abs(d));

       color += texture(screenTexture, TexCoords + dir * d).rgb;
    }
    if (drunk >= 2) {
        color += fluctuateColors(color);
    }
    if (drunk >= 3) {
        color = blink(1.5, color);    
    }

    gl_FragColor = vec4(color, 1.0);
}
