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
    //基础光照贴图
    sampler2D diffuse;
    sampler2D specular;
    samplerCube envTexture;
    sampler2D normalMap;//法线贴图
    sampler2D shadowMap;
    //SSAO环境光遮蔽
    sampler2D gPositionDepth;
    sampler2D gNormal;
    sampler2D gAlbedo;
    sampler2D ssao;
    float shininess;
    bool active_diffuse;//渲染模式
    bool active_specular;
    bool active_envTexture;
    bool active_normalMap;
    bool active_shadowMap;
    bool active_ssao;
}; 

//定向光
//------
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

//点光源
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
//代表四个待填充数据的PointLight结构体
#define NR_POINT_LIGHTS 4

//聚光源
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

//全局信息
//--------
struct AttritubeActive{
    bool dirLight;
    bool pointLight;
    bool spotLight;
};

//外部数据接入
//------------
uniform vec3 viewPos;
uniform Material material;
//uniform samplerCube depthMap;//当使用万向阴影映射时则需要用到该立方体贴图
//uniform float far_plane;//同时再使用万向阴影映射时还需要映入视锥平面的远面值
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform AttritubeActive attritubeActive;

//函数定义
//--------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);
float ShadowCalculation(vec3 fragPos);//当使用万向阴影映射时则需要用到该阴影计算函数
vec3 convolution(sampler2D _textureK, vec3 _TexCoords);//3x3卷积

bool blinn;//blinn光照
bool blending;//混合

float shadow_xxx;
float AmbientOcclusion;//环境光遮蔽的值

const float NEAR = 0.1; // 投影矩阵的近平面
const float FAR = 100.0f; // 投影矩阵的远平面
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}
void LoadSSAO()
{
    // 储存片段的位置矢量到第一个G缓冲纹理
    gPositionDepth.xyz = FragPos;
    // 储存线性深度到gPositionDepth的alpha分量
    gPositionDepth.a = LinearizeDepth(gl_FragCoord.z); 
    // 储存法线信息到G缓冲
    gNormal = normalize(Normal);
    // 和漫反射颜色
    //gAlbedoSpec.rgb = FragColor.rgb;
}

void main()
{
    //数据初始化
    //----------
    //是否启用blinn-phong模型
    blinn = true;
    blending = true;

    // 属性
    //-----
    vec3 norm = normalize(Normal);
    //是否启用法线贴图
    if(material.active_normalMap)
    {
        // 从法线贴图范围[0,1]获取法线
        norm = texture(material.normalMap, TexCoords).rgb;
        // 将法线向量转换为范围[-1,1]
        norm = normalize(norm * 2.0 - 1.0);
    }
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 viewDirX = normalize(FragPos - viewPos);//观察位置减去摄像机位置

    // 计算阴影
    //----------
    float shadow = 0;
    float xxx = 0;
    shadow_xxx = 0.3;
    if(material.active_shadowMap)
    {
        //计算出一个shadow值，当fragment在阴影中时是1.0，在阴影外是0.0
        shadow = ShadowCalculation(FragPosLightSpace, norm, normalize(spotLight.position - FragPos));//由光的视角进行计算当前片段是否在阴影中，传入参数：光空间向量、表面法向量、光线角度
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

    // 第一阶段：定向光照
    vec3 result;
    //if(attritubeActive.dirLight)
    {
        result = CalcDirLight(dirLight, norm, viewDir); 
    }
    //vec3 result = vec3(0.5f)  * vec3(texture(material.diffuse, TexCoords));
    // 第二阶段：点光源
    if(attritubeActive.pointLight)
    {
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // 第三阶段：聚光
    if(attritubeActive.spotLight)
    {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    }

    //环境映射
    //---------
    //初始化反射颜色
    vec4    reflectColor = vec4(0.0, 0.0, 0.0, 1.0);
    //反射量
    float   relefctIntensity = result.r;//texture_height//material.height
    //反射
    if(true)
    {
    //判断映射量觉得是否启用环境映射
        if (relefctIntensity > 0.1) // 决定是否开启//relefctIntensity > 0.1
        {
            if(!blinn)
            {
                //冯氏镜面映射
                //------------
                //先计算观察方向向量
                viewDirX = normalize(FragPos - viewPos);//观察位置减去摄像机位置
                //映射方向向量//reflect()计算映射位置
                vec3 reflectDir = reflect(viewDirX, normalize(FragNormal));//material.normal//FragNormal
                //得到映射过后的颜色
                reflectColor = texture(material.envTexture, reflectDir); // 使用反射向量采样环境纹理 使用强度系数控制
            }
            else
            {
                //Blinn-Phong光照
                //vec3 lightDir   = normalize(light.position - FragPos);
                //vec3 viewDir    = normalize(FragPos - viewPos);//观察位置减去摄像机位置
                vec3 lightDir = normalize(spotLight.position - FragPos);
                vec3 halfwayDir = normalize(lightDir + viewDirX);//获取半程向量的方法很简单，只需要将光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)就可以了
                float reflectDir = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);//面光分量的实际计算只不过是对表面法线和半程向量进行一次约束点乘(Clamped Dot Product)，让点乘结果不为负，从而获取它们之间夹角的余弦值，之后我们对这个值取反光度次方
                //得到映射过后的颜色
                reflectColor = texture(material.envTexture, result); // 使用反射向量采样环境纹理 使用强度系数控制
            }
        }
    }
    //折射
    if(false)
    {
        //折射率
        float indexRation = 1.00 / 1.52;
        vec3 viewDirZ = normalize(FragPos - viewPos); // 注意这里向量从观察者位置指出
        vec3 refractDir = refract(viewDirZ, normalize(FragNormal), indexRation);
        reflectColor = texture(material.envTexture, refractDir);   // 使用 折射向量 采样环境纹理
    }
    

    // Reinhard色调映射
    //------------------
    //result = result / (result + vec3(1.0));//这个算法是倾向明亮的区域的，暗的区域会不那么精细也不那么有区分度。
    // 曝光色调映射
    //float exposure = 1.0;//曝光值(0.1, 1.0, 5.0)//高曝光值会使隧道的黑暗部分显示更多的细节，然而低曝光值会显著减少黑暗区域的细节，但允许我们看到更多明亮区域的细节
    //result = vec3(1.0) - exp(-result * exposure);
    // Gamma校正
    //const float gamma = 2.2f;
    //result = pow(result, vec3(1.0 / gamma));

    // 混合
    //------
    if(blending)
    {
        //获得当前片段颜色
        vec4 texColor = texture(material.diffuse, TexCoords);
        if(texColor.a < 0.1)
            //它会保证片段不会被进一步处理，所以就不会进入颜色缓冲。
            //就能够在片段着色器中检测一个片段的alpha值是否低于某个阈值，如果是的话，则丢弃这个片段，就好像它不存在一样
            discard;
        gAlbedoSpec.rgb = result;
        FragColor = vec4(result, texture(material.diffuse, TexCoords).a);//vec4(result, 1.0) + reflectColor * relefctIntensity; + vec4(reflectColor.rgb, 1.0)
    }
    //FragColor = vec4(1.0,1.0,1.0,1.0);
    //FragColor = vec4(vec3(shadow), 1.0);

    // 计算ssao
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

    // 泛光
    //-----
    // * (0.5 + 1 - xxx)
    //检查片段输出是否高于阈值，如果高于阈值，则输出为亮度颜色
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

//定向光函数
//----------
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //光源方向标准化
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    // -----------
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //冯氏光照
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong光照
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//获取半程向量的方法很简单，只需要将光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)就可以了
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//面光分量的实际计算只不过是对表面法线和半程向量进行一次约束点乘(Clamped Dot Product)，让点乘结果不为负，从而获取它们之间夹角的余弦值，之后我们对这个值取反光度次方
    }
    //环境光遮蔽
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // 合并结果
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular) * (shadow_xxx * 0.5);// + specular// * (shadow_xxx * 0.5)
}

//点光源函数
//----------
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //光源方向
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.0f;
    if(!blinn)
    {
        //冯氏光照
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong光照
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//获取半程向量的方法很简单，只需要将光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)就可以了
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//面光分量的实际计算只不过是对表面法线和半程向量进行一次约束点乘(Clamped Dot Product)，让点乘结果不为负，从而获取它们之间夹角的余弦值，之后我们对这个值取反光度次方
    }
    // 衰减
    // -----
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    //环境光遮蔽
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // 合并结果
    //---------
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient  + diffuse + specular);// + specular
}

//聚光源函数
//-----------
// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    // specular shading
    // -----------
    float spec = 0.0f;
    if(!blinn)
    {
        //冯氏光照
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }else{
        //Blinn-Phong光照
        //vec3 lightDir   = normalize(light.position - FragPos);
        //vec3 viewDir    = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);//获取半程向量的方法很简单，只需要将光线的方向向量和观察向量加到一起，并将结果正规化(Normalize)就可以了
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);//面光分量的实际计算只不过是对表面法线和半程向量进行一次约束点乘(Clamped Dot Product)，让点乘结果不为负，从而获取它们之间夹角的余弦值，之后我们对这个值取反光度次方
    }
    // 衰减 attenuation 
    // ------------
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 柔化边缘 spotlight intensity 
    // --------------------
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 环境光遮蔽
    //----------
    if(material.active_ssao)
    {
        //light.ambient = light.ambient * 1;// *= vec3(0.3 * AmbientOcclusion);
    }
    // 合并结果combine results
    // ----------------
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);// + specular
}

//阴影计算
//--------
float ShadowCalculation(vec4 fragPosLightSpace, vec3 _normal, vec3 _lightDir)
{
    // 执行透视除法:把光空间片段位置转换为裁切空间的标准化设备坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;//由于裁切空间的FragPosLightSpace并不会通过gl_Position传到片段着色器里，我们必须自己做透视除法：返回了片段在光空间的-1到1的范围
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    //float closestDepth = texture(material.shadowMap, projCoords.xy).r; //得到光的位置视野下最近的深度
    float closestDepth = convolution(material.shadowMap, projCoords).r;
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    //使用阴影偏移
    float bias = 0.002;//0.005
    //float bias = max(0.05 * (1.0 - dot(_normal, _lightDir)), 0.005);
    // 检查当前片段是否在阴影中
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    //FragColor = vec4(vec3(shadow), 1.0);

    return shadow;
}

/*
//阴影计算-万向阴影映射
//--------
float ShadowCalculation(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos; 
    //closestDepth是光源和它最接近的可见fragment之间的标准化的深度值。
    float closestDepth = texture(depthMap, fragToLight).r;//方向向量不需要是单位向量，所以无需对它进行标准化
    //closestDepth值现在在0到1的范围内了，所以我们先将其转换回0到far_plane的范围，这需要把他乘以far_plane：
    closestDepth *= far_plane;
    //获取当前fragment和光源之间的深度值，我们可以简单的使用fragToLight的长度来获取它，这取决于我们如何计算立方体贴图中的深度值：
    float currentDepth = length(fragToLight);
    //现在我们可以将两个深度值对比一下，看看哪一个更接近，以此决定当前的fragment是否在阴影当中。我们还要包含一个阴影偏移，所以才能避免阴影失真，
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    //如果投影向量的z坐标大于1.0，就把shadow的值强制设为0.0,以解决远平面黑暗的问题
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
*/
//一个简单的把深度缓冲显示出来的技巧：在ShadowCalculation函数中计算标准化的closestDepth变量，把变量显示为：
//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);


//模糊
float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
const float offset = 1.0 / 300.0;  
vec3 convolution(sampler2D _textureK, vec3 _TexCoords)//卷积
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
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