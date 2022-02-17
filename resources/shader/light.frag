#version 330 core
out vec4 FragColor;

in vec2 TexCoord;//��������
in vec3 FragPos;//Ƭ��λ��
in vec3 Normal;//������

uniform vec4 color;
void main()
{
	if(color != vec4(0))
		FragColor = color;
	else
		FragColor = vec4(1.0,1.0,1.0,1.0);
}