#version 120

attribute vec3 coord;
attribute vec2 texcoord;
varying vec2 f_texcoord;

uniform mat4 matrix;

void main() {
    vec4 offset = vec4(-1.0, 1.0, -2, 0.0);
    vec4 cameraPos = vec4(coord, 1.0) + offset;

    gl_Position = matrix * cameraPos;
  
    f_texcoord = texcoord;
}
