#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

[...]

void main()
{            
    [...]
    //���Ƚ��г����������㲢������
    FragColor = vec4(lighting, 1.0f);
    //���Ƭ������Ƿ������ֵ�����������ֵ�������Ϊ������ɫ
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
}