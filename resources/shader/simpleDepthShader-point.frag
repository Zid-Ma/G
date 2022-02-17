#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;//���λ������
uniform float far_plane;//��׶��Զƽ��ֵ

//�����Լ�����ȣ������Ⱦ���ÿ��fragmentλ�ú͹�Դλ��֮������Ծ��롣�����Լ������ֵʹ��֮�����Ӱ�������ֱ�ۡ�
void main()
{
    // get distance between fragment and light source//��ȡ��Ƭ�͹�Դ֮��ľ���
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane//ӳ�䵽[0��1] ����ͨ������Զ_ƽ��
    lightDistance = lightDistance / far_plane;

    // write this as modified depth//д��Ϊfragment�����ֵ��modified/���Ľ��ģ�
    gl_FragDepth = lightDistance;
}