#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 gPositionDepth;
layout (location = 3) out vec3 gNormal;
layout (location = 4) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec3 FragNormal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct Material {
    //����������ͼ
    sampler2D diffuse;
    sampler2D specular;
    samplerCube envTexture;
    sampler2D normalMap;//������ͼ
    sampler2D shadowMap;
    //SSAO�������ڱ�
    sampler2D gPositionDepth;
    sampler2D gNormal;
    sampler2D gAlbedo;
    sampler2D ssao;
    float shininess;
    bool active_diffuse;//��Ⱦģʽ
    bool active_specular;
    bool active_envTexture;
    bool active_normalMap;
    bool active_shadowMap;
    bool active_ssao;
}; 

//�����
//------
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//���Դ
//------
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
//�����ĸ���������ݵ�PointLight�ṹ��
#define NR_POINT_LIGHTS 4

//�۹�Դ
//------
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//ȫ����Ϣ
//--------
struct AttritubeActive{
    bool dirLight;
    bool pointLight;
    bool spotLight;
};

//�ⲿ���ݽ���
//------------
uniform vec3 viewPos;
uniform Material material;
//uniform samplerCube depthMap;//��ʹ��������Ӱӳ��ʱ����Ҫ�õ�����������ͼ
//uniform float far_plane;//ͬʱ��ʹ��������Ӱӳ��ʱ����Ҫӳ����׶ƽ���Զ��ֵ
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform AttritubeActive attritubeActive;

//��������
//--------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalculation(vec3 fragPos);//��ʹ��������Ӱӳ��ʱ����Ҫ�õ�����Ӱ���㺯��
vec3 convolution(sampler2D _textureK, vec3 _TexCoords);//3x3���

bool blinn;//blinn����
bool blending;//���

float shadow_xxx;
float AmbientOcclusion;//�������ڱε�ֵ

const float NEAR = 0.1; // ͶӰ����Ľ�ƽ��
const float FAR = 100.0f; // ͶӰ�����Զƽ��
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // �ص�NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}
void LoadSSAO()
{
    // ����Ƭ�ε�λ��ʸ������һ��G��������
    gPositionDepth.xyz = FragPos;
    // ����������ȵ�gPositionDepth��alpha����
    gPositionDepth.a = LinearizeDepth(gl_FragCoord.z); 
    // ���淨����Ϣ��G����
    gNormal = normalize(Normal);
    // ����������ɫ
    //gAlbedoSpec.rgb = FragColor.rgb;
}

void main()
{
    //���ݳ�ʼ��
    //----------
    //�Ƿ�����blinn-phongģ��
    blinn = true;
    blending = true;

    // ����
    //-----
    vec3 norm = normalize(Normal);
    //�Ƿ����÷�����ͼ
    if(material.active_normalMap)
    {
        // �ӷ�����ͼ��Χ[0,1]��ȡ����
        norm = texture(material.normalMap, TexCoords).rgb;
        // ����������ת��Ϊ��Χ[-1,1]
        norm = normalize(norm * 2.0 - 1.0);
    }
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 viewDirX = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��

    // ������Ӱ
    //----------
    float shadow = 0;
    float xxx = 0;
    shadow_xxx = 0.3;
    if(material.active_shadowMap)
    {
        //�����һ��shadowֵ����fragment����Ӱ��ʱ��1.0������Ӱ����0.0
        shadow = ShadowCalculation(FragPosLightSpace, norm, normalize(spotLight.position - FragPos));//�ɹ���ӽǽ��м��㵱ǰƬ���Ƿ�����Ӱ�У������������ռ����������淨���������߽Ƕ�
        if(shadow == 0.0)
        {
            xxx = 1;
            shadow_xxx = 1;
        }
    }
    else
    {
        xxx = 1;
        shadow_xxx = 1;
    }

    // ��һ�׶Σ��������
    vec3 result;
    //if(attritubeActive.dirLight)
    {
        result = CalcDirLight(dirLight, norm, viewDir); 
    }
    //vec3 result = vec3(0.5f)  * vec3(texture(material.diffuse, TexCoords));
    // �ڶ��׶Σ����Դ
    if(attritubeActive.pointLight)
    {
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // �����׶Σ��۹�
    if(attritubeActive.spotLight)
    {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    }

    //����ӳ��
    //---------
    //��ʼ��������ɫ
    vec4    reflectColor = vec4(0.0, 0.0, 0.0, 1.0);
    //������
    float   relefctIntensity = result.r;//texture_height//material.height
    //����
    if(true)
    {
    //�ж�ӳ���������Ƿ����û���ӳ��
        if (relefctIntensity > 0.1) // �����Ƿ���//relefctIntensity > 0.1
        {
            if(!blinn)
            {
                //���Ͼ���ӳ��
                //------------
                //�ȼ���۲췽������
                viewDirX = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��
                //ӳ�䷽������//reflect()����ӳ��λ��
                vec3 reflectDir = reflect(viewDirX, normalize(FragNormal));//material.normal//FragNormal
                //�õ�ӳ��������ɫ
                reflectColor = texture(material.envTexture, reflectDir); // ʹ�÷������������������� ʹ��ǿ��ϵ������
            }
            else
            {
                //Blinn-Phong����
                //vec3 lightDir   = normalize(light.position - FragPos);
                //vec3 viewDir    = normalize(FragPos - viewPos);//�۲�λ�ü�ȥ�����λ��
                vec3 lightDir = normalize(spotLight.position - FragPos);
                vec3 halfwayDir = normalize(lightDir + viewDirX);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
                float reflectDir = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
                //�õ�ӳ��������ɫ
                reflectColor = texture(material.envTexture, result); // ʹ�÷������������������� ʹ��ǿ��ϵ������
            }
        }
    }
    //����
    if(false)
    {
        //������
        float indexRation = 1.00 / 1.52;
        vec3 viewDirZ = normalize(FragPos - viewPos); // ע�����������ӹ۲���λ��ָ��
        vec3 refractDir = refract(viewDirZ, normalize(FragNormal), indexRation);
        reflectColor = texture(material.envTexture, refractDir);   // ʹ�� �������� ������������
    }
    

    // Reinhardɫ��ӳ��
    //------------------
    //result = result / (result + vec3(1.0));//����㷨����������������ģ���������᲻��ô��ϸҲ����ô�����ֶȡ�
    // �ع�ɫ��ӳ��
    //float exposure = 1.0;//�ع�ֵ(0.1, 1.0, 5.0)//���ع�ֵ��ʹ����ĺڰ�������ʾ�����ϸ�ڣ�Ȼ�����ع�ֵ���������ٺڰ������ϸ�ڣ����������ǿ����������������ϸ��
    //result = vec3(1.0) - exp(-result * exposure);
    // GammaУ��
    //const float gamma = 2.2f;
    //result = pow(result, vec3(1.0 / gamma));

    // ���
    //------
    if(blending)
    {
        //��õ�ǰƬ����ɫ
        vec4 texColor = texture(material.diffuse, TexCoords);
        if(texColor.a < 0.1)
            //���ᱣ֤Ƭ�β��ᱻ��һ���������ԾͲ��������ɫ���塣
            //���ܹ���Ƭ����ɫ���м��һ��Ƭ�ε�alphaֵ�Ƿ����ĳ����ֵ������ǵĻ����������Ƭ�Σ��ͺ�����������һ��
            discard;
        gAlbedoSpec.rgb = result;
        FragColor = vec4(result, texture(material.diffuse, TexCoords).a);//vec4(result, 1.0) + reflectColor * relefctIntensity; + vec4(reflectColor.rgb, 1.0)
    }
    //FragColor = vec4(1.0,1.0,1.0,1.0);
    //FragColor = vec4(vec3(shadow), 1.0);

    // ����ssao
    //---------
    // Retrieve data from g-buffer
    AmbientOcclusion = 1;
    if(material.active_ssao)
    {
        LoadSSAO();
        //vec3 FragPos = texture(material.gPositionDepth, TexCoords).rgb;
        //vec3 Normal = texture(material.gNormal, TexCoords).rgb;
        //vec3 Diffuse = texture(material.gAlbedo, TexCoords).rgb;
        //AmbientOcclusion = texture(material.ssao, TexCoords).r;
    }
    //AmbientOcclusion = 1;

    // ����
    //-----
    // * (0.5 + 1 - xxx)
    //���Ƭ������Ƿ������ֵ�����������ֵ�������Ϊ������ɫ
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
    else{
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

FragColor = vec4(1.0f);
    //material.diffuse//screenTexture
    if(texture(material.diffuse, TexCoords).r < 0.2f)
        discard;
}

//����⺯��
//----------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //��Դ�����׼��
    vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    // -----------
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }
    //�������ڱ�
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // �ϲ����
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular) * (shadow_xxx * 0.5);// + specular// * (shadow_xxx * 0.5)
}

//���Դ����
//----------
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //��Դ����
    vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }
    // ˥��
    // -----
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    //�������ڱ�
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // �ϲ����
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient  + diffuse + specular);// + specular
}

//�۹�Դ����
//-----------
// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    // specular shading
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //���Ϲ���
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong����
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ�����
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//��������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη�
    }
    // ˥�� attenuation 
    // ------------
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // �ữ��Ե spotlight intensity 
    // --------------------
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // �������ڱ�
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // �ϲ����combine results
    // ----------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);// + specular
}

//��Ӱ����
//--------
float ShadowCalculation(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir)
{
    // ִ��͸�ӳ���:�ѹ�ռ�Ƭ��λ��ת��Ϊ���пռ�ı�׼���豸����
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;//���ڲ��пռ��FragPosLightSpace������ͨ��gl_Position����Ƭ����ɫ������Ǳ����Լ���͸�ӳ�����������Ƭ���ڹ�ռ��-1��1�ķ�Χ
    // �任��[0,1]�ķ�Χ
    projCoords = projCoords * 0.5 + 0.5;
    // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
    //float closestDepth = texture(material.shadowMap, projCoords.xy).r; //�õ����λ����Ұ����������
    float closestDepth = convolution(material.shadowMap, projCoords).r;
    // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
    float currentDepth = projCoords.z;
    //ʹ����Ӱƫ��
    float bias = 0.002;//0.005
    //float bias = max(0.05 * (1.0 - dot(_normal, _lightDir)), 0.005);
    // ��鵱ǰƬ���Ƿ�����Ӱ��
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    //FragColor = vec4(vec3(shadow), 1.0);

    return shadow;
}

/*
//��Ӱ����-������Ӱӳ��
//--------
float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos; 
    //closestDepth�ǹ�Դ������ӽ��Ŀɼ�fragment֮��ı�׼�������ֵ��
    float closestDepth = texture(depthMap, fragToLight).r;//������������Ҫ�ǵ�λ��������������������б�׼��
    //closestDepthֵ������0��1�ķ�Χ���ˣ����������Ƚ���ת����0��far_plane�ķ�Χ������Ҫ��������far_plane��
    closestDepth *= far_plane;
    //��ȡ��ǰfragment�͹�Դ֮������ֵ�����ǿ��Լ򵥵�ʹ��fragToLight�ĳ�������ȡ������ȡ����������μ�����������ͼ�е����ֵ��
    float currentDepth = length(fragToLight);
    //�������ǿ��Խ��������ֵ�Ա�һ�£�������һ�����ӽ����Դ˾�����ǰ��fragment�Ƿ�����Ӱ���С����ǻ�Ҫ����һ����Ӱƫ�ƣ����Բ��ܱ�����Ӱʧ�棬
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    //���ͶӰ������z�������1.0���Ͱ�shadow��ֵǿ����Ϊ0.0,�Խ��Զƽ��ڰ�������
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
*/
//һ���򵥵İ���Ȼ�����ʾ�����ļ��ɣ���ShadowCalculation�����м����׼����closestDepth�������ѱ�����ʾΪ��
//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);


//ģ��
float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
const float offset = 1.0 / 300.0;  
vec3 convolution(sampler2D _textureK, vec3 _TexCoords)//���
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
        sampleTex[i] = vec3(texture(_textureK, _TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return col;
}