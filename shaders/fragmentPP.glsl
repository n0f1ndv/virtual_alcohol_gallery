#version 330 core

in vec2 TexCoords;

// TODO: I think I didn't set screenTexture but for some reason it works ;0;
uniform sampler2D screenTexture;
uniform float time;

void main()
{ 
    float strength = 5.0;

    gl_FragColor = vec4(vec3((sin(strength * time) + 1) / 2 * (1.0 - texture(screenTexture, TexCoords))), 1.0);
}