#version 300 es
precision mediump float;
in vec3 vColor;
out vec4 FragColor;
void main()
{
	if (vColor == vec3(0,0,0)) {
		discard;
	}
	FragColor = vec4(vColor, 1.0);
}
