#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;

// TODO: I think I didn't set screenTexture but for some reason it works ;0;
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

void main()
{ 
    vec3 color = texture(screenTexture, TexCoords).rgb;

    // Uncomment only one at a time
    // gl_FragColor = negative(color);
    // gl_FragColor = grayscale(color);
    // gl_FragColor = blink(2.5, color);
    // gl_FragColor = fluctuateColors(color);
    gl_FragColor = vec4(color, 1.0);
}