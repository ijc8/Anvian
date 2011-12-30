#version 120

attribute vec3 coord;
attribute vec2 texcoord;
varying vec2 f_texcoord;

uniform mat4 worldMatrix;
uniform mat4 cameraMatrix;

void main() {
    vec4 cameraPos = vec4(coord, 1.0);

    gl_Position = cameraMatrix * worldMatrix * cameraPos; 
    f_texcoord = texcoord;
}
