#version 330 core
layout (location = 0) in vec3 aPos;//需要接收的参数,位置
layout (location = 1) in vec3 aNormal;//法向量
layout (location = 2) in vec2 aTexCoord;//需要接收的参数,纹理位置

out vec2 TexCoords;//需要输出的参数，纹理位置
out vec3 FragPos;
out vec3 Normal;
out vec3 FragNormal;
out vec4 FragPosLightSpace;

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵
uniform mat4 lightSpaceMatrix;//光空间变换矩阵

void main()
{
	TexCoords = vec2(aTexCoord.x, aTexCoord.y);//输出到片段着色器中的纹理位置参数
	FragPos = vec3(model * vec4(aPos, 1.0));//片段位置 
	Normal = aNormal;//输出到片段着色器
	//法线矩阵
	//NormalMat = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);//最终渲染的顶点位置参数

    //映射
    //------------  
    //我们现在使用了一个法向量，所以我们将再次使用法线矩阵(Normal Matrix)来变换它们。
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    // 计算法向量经过模型变换后值
    FragNormal = normalMatrix * aNormal; 

    //阴影
    //------
    //把世界空间顶点位置转换为光空间向量
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    //当使用万向阴影映射时则不再需要将他的位置向量变换到光空间

}