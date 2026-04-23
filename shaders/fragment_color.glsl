#version 330

out vec4 pixel_color;

in vec4 ver_color;

void main(void) {
    pixel_color = ver_color;
}
