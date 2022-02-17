#version 330 core
out vec4 FragColor;

in vec2 TexCoord;//纹理坐标
in vec3 FragPos;//片段位置
in vec3 Normal;//法向量

void main()
{
	FragColor = vec4(1.0,1.0,1.0,1.0);
}