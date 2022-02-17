#version 330 core
//基于高斯模糊的像素着色器

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
//各个颜色对应的权重值
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0); //获取单个texel的大小 gets size of single texel
    vec3 result = texture(image, TexCoords).rgb * weight[0]; //当前片段的作用大小 current fragment's contribution
    //判断是否为水平方向
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)//5次模糊处理
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
