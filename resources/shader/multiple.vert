#version 330 core
layout (location = 0) in vec3 aPos;//��Ҫ���յĲ���,λ��
layout (location = 1) in vec3 aNormal;//������
layout (location = 2) in vec2 aTexCoord;//��Ҫ���յĲ���,����λ��

out vec2 TexCoords;//��Ҫ����Ĳ���������λ��
out vec3 FragPos;
out vec3 Normal;
out vec3 FragNormal;
out vec4 FragPosLightSpace;

uniform mat4 model;//ģ�;���
uniform mat4 view;//�۲����
uniform mat4 projection;//ͶӰ����
uniform mat4 lightSpaceMatrix;//��ռ�任����

void main()
{
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);//�����Ƭ����ɫ���е�����λ�ò���
	FragPos = vec3(model * vec4(aPos, 1.0));//Ƭ��λ�� 
	Normal = aNormal;//�����Ƭ����ɫ��
	//���߾���
	//NormalMat = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);//������Ⱦ�Ķ���λ�ò���

    //ӳ��
    //------------  
    //��������ʹ����һ�����������������ǽ��ٴ�ʹ�÷��߾���(Normal Matrix)���任���ǡ�
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    // ���㷨��������ģ�ͱ任��ֵ
    FragNormal = normalMatrix * aNormal; 

    //��Ӱ
    //------
    //������ռ䶥��λ��ת��Ϊ��ռ�����
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    //��ʹ��������Ӱӳ��ʱ������Ҫ������λ�������任����ռ�

}