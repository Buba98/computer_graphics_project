#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    float visible;
    float speed;
    float ar;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec2 outHandUV;

void main() {

    vec2 pos = inPosition * vec2(1.0f, ubo.ar);
    pos *= .20f;
    pos -= vec2(-.75f, -.55f);


    gl_Position = vec4(pos * ubo.visible, 0.5f, 1.0f);
    vec2 uvCentered = inUV - vec2(0.5f, 0.5f);

    float angle = -ubo.speed * 1.7;

    float x = uvCentered.x * cos(angle) - uvCentered.y * sin(angle);
    float y = uvCentered.x * sin(angle) + uvCentered.y * cos(angle);

    vec2 rotatedUVCentered = vec2(x, y);

    vec2 rotatedUV = rotatedUVCentered + vec2(0.5f, 0.5f);

    outUV = inUV;
    outHandUV = rotatedUV;
}