#version 330 core
//���ڸ�˹ģ����������ɫ��

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
//������ɫ��Ӧ��Ȩ��ֵ
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0); //��ȡ����texel�Ĵ�С gets size of single texel
    vec3 result = texture(image, TexCoords).rgb * weight[0]; //��ǰƬ�ε����ô�С current fragment's contribution
    //�ж��Ƿ�Ϊˮƽ����
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)//5��ģ������
        {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
