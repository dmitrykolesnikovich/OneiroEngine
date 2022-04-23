#version 450

layout(location = 0) in vec2 WindowSize;
layout(location = 1) in float uDelta;

layout(location = 0) out vec4 FragColor;

void main() {
    vec2 uv = gl_FragCoord.xy / WindowSize;
    vec4 color = vec4(0);
    color.rg += cos(uv + uDelta) * cos(uDelta);
    color.gb += sin(uv + uDelta) * sin(uDelta);
    FragColor = color;
    FragColor.a = 1.0;
}
