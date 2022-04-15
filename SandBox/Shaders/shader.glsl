// VERTEX SHADER
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0);
}

// TFRAGMENT SHADER
out vec4 FragColor;
uniform float uTime;
uniform vec2 uSize;
void main()
{
	vec2 uv = gl_FragCoord.xy / uSize;
	vec4 color = vec4(0);
	color.rg += cos(uv + uTime) * cos(uTime);
	color.gb += sin(uv + uTime) * sin(uTime);
	FragColor = color;
	FragColor.a = 1.0;
}