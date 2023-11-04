#version 440

uniform sampler2D atlas;
in vec2 uv;

out vec4 out_Color;

void main() {
    out_Color = texture(atlas, uv);
}
