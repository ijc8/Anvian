#version 120

varying vec2 f_texcoord;
uniform sampler2D mytexture;

void main() {
    gl_FragColor = texture2D(mytexture, f_texcoord);
}
