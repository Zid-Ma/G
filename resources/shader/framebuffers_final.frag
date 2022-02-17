#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

vec3 convolution(sampler2D textureK);

//�������-������ɫ�ϳ�
uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform bool bloom = true;//�Ƿ�������
uniform float exposure = 1.0;//�ع�ֵ
uniform sampler2D ssao;//SSAO��ͼ
uniform bool active_ssao;
uniform sampler2D gPositionDepth;//���
uniform sampler2D gNormal;//����
uniform sampler2D gAlbedo;//������

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
uniform Light light;

//�ӳ���ɫ
vec3 SSAO();

void main()
{
    //gamma��ֵ
    const float gamma = 2.2;
    //������������
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    //gammaУ��
    //---------  
    //result = pow(result, vec3(1.0 / gamma));
    //result = convolution(screenTexture);
    //SSAO
    //-----
    vec3 sss;
    if(active_ssao)
    {
        sss = SSAO();
        if(sss != vec3(1.0f))
        {
            //result += normalize(sss);//((sss + vec3(1.0f)) * 0.5f);//vec3(AmbientOcclusion * 0.3f);
            //result *= 0.3f;
        }
        //result = vec3(1.0) - exp(-result * exposure);
    }

    FragColor = vec4(result, 1.0f);//texture(gAlbedo, TexCoords).rgb

    //FragColor = vec4(vec3(hdrColor.r), 1.0f);
}

//ģ��
float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
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

vec3 SSAO()
{
    // Retrieve data from g-buffer
    vec3 FragPos = texture(gPositionDepth, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    float AmbientOcclusion = texture(ssao, TexCoords).r;//;
    float ax = (((AmbientOcclusion) * 0.33f) - 1) * -1;

    // ��������Then calculate lighting as usual
    vec3 ambient = vec3(0.3f * AmbientOcclusion); // <-- this is where we use ambient occlusion
    vec3 lighting  = ambient; 

    vec3 viewDir  = normalize(-FragPos); // Viewpos is (0.0.0)
    // Diffuse
    vec3 lightDir = normalize(light.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
    vec3 specular = light.Color * spec;
    // ˥�� Attenuation
    float distance = length(light.Position - FragPos);
    float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
    diffuse *= attenuation;
    //Diffuse *= AmbientOcclusion;
    //specular *= attenuation;
    lighting += Diffuse;// + specular;

    return lighting;
}