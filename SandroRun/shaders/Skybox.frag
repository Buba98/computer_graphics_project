#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform SkyboxUniformBlock {
    int time_of_day;
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(set = 0, binding = 1) uniform samplerCube cubeMap1;
layout(set = 0, binding = 2) uniform samplerCube cubeMap2;
layout(set = 0, binding = 3) uniform samplerCube cubeMap3;

layout(location = 0) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {
    if (ubo.time_of_day == 0){
        outColor = texture(cubeMap1, fragPos);
    } else if (ubo.time_of_day == 1){
        outColor  = texture(cubeMap2, fragPos);
    } else {
        outColor  = texture(cubeMap3, fragPos);
    }
}