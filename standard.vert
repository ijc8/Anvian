#version 120

attribute vec3 coord;
attribute vec2 texcoord;
varying vec2 f_texcoord;

uniform float zNear;
uniform float zFar;
uniform float frustumScale;

void main() {
    vec4 offset = vec4(-1.5, -1.5, -3.0, 0.0);
    vec4 cameraPos = vec4(coord, 1.0) + offset;
    vec4 clipPos;

    clipPos.xy = cameraPos.xy * frustumScale;
    clipPos.z = cameraPos.z * (zNear + zFar) / (zNear - zFar);
    clipPos.z += 2 * zNear * zFar / (zNear - zFar);
    clipPos.w = -cameraPos.z;

    gl_Position = clipPos;
    
    f_texcoord = texcoord;
}
