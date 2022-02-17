#version 330 core
out vec4 FragColor;
//out vec4 BrightColor;

in vec2 TexCoords;

uniform vec4 color;
uniform sampler2D screenTexture;
vec3 convolution(sampler2D textureK);
vec3 stroke(sampler2D textureS);
void main()
{ 
    //FragColor = texture(screenTexture, TexCoords);
    int alpha = 1;
    vec3 rgb = vec3(0.0f);
    if(texture(screenTexture, TexCoords).r < 0.2f)
    {
        vec3 checkStroke = stroke(screenTexture);
        if(checkStroke == vec3(0.0f))
        {
            discard;
        }
        else
        {
            //rgb = checkStroke;
            rgb = texture(screenTexture, TexCoords).rgb;
        }
    }
    else
    {
        rgb = texture(screenTexture, TexCoords).rgb;//convolution(screenTexture)
    }
        //alpha = 0;
    if(color == vec4(0.0f))
    {

        FragColor = vec4(rgb,alpha);
    }
    else
    {
        FragColor = color;//vec4(color,alpha);
    }
}

//ģ��
float Xkernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);
//��
float kernel[9] = float[](
   -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);
//��Ե���
float Zkernel[9] = float[](//ԭ�����Ϊx
   1, 2, -1,
    2,  0, -2,
    1, -2, -1
);
const float offset = 1.0 / 300.0;  
vec3 convolution(sampler2D textureK)//���
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(textureK, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return col;
}

//���
vec3 stroke(sampler2D textureS)
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(textureS, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        if(sampleTex[i].r > 0.2f)
            return vec3(0.3f);
    return vec3(0.0f);
}