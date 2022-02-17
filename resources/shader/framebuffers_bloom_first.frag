#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

[...]

void main()
{            
    [...]
    //首先进行常规照明计算并输出结果
    FragColor = vec4(lighting, 1.0f);
    //检查片段输出是否高于阈值，如果高于阈值，则输出为亮度颜色
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
}