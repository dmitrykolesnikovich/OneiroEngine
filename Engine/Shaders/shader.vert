#version 450

layout(binding = 1) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec2 windowSize;
    float delta;
} ubo;

layout(location = 0) in vec2 aPos;

layout(location = 0) out vec2 WindowSize;
layout(location = 1) out float uDelta;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(aPos, 0.0, 1.0);
    WindowSize = ubo.windowSize;
    uDelta = ubo.delta;
}
