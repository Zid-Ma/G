#version 330 core
layout (location = 0) in vec3 aPos;//��Ҫ���յĲ���,λ��
layout (location = 1) in vec3 aNormal;//������
layout (location = 2) in vec2 aTexCoord;//��Ҫ���յĲ���,����λ��

out vec2 TexCoord;//��Ҫ����Ĳ���������λ��
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;//ģ�;���
uniform mat4 view;//�۲����
uniform mat4 projection;//ͶӰ����

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);//������Ⱦ�Ķ���λ�ò���
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);//�����Ƭ����ɫ���е�����λ�ò���
	FragPos = vec3(model * vec4(aPos, 1.0));//Ƭ��λ��
	Normal = aNormal;//�����Ƭ����ɫ��
	//���߾���
	//NormalMat = mat3(transpose(inverse(model))) * aNormal;
}