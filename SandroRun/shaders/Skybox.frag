#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform SkyboxUniformBlock {
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(set = 0, binding = 1) uniform samplerCube cubeMap;

layout(location = 0) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(cubeMap, fragPos);
}