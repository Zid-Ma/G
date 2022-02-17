#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    //将一个单独模型的一个顶点，使用lightSpaceMatrix变换到光空间中
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}


/*
#version 330 core
out vec4 color;
in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

//使用透视深度缓冲时，将深度值转化为如正交深度缓冲时的线性深度值
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{             
    float depthValue = texture(depthMap, TexCoords).r;
    color = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    // color = vec4(vec3(depthValue), 1.0); // orthographic
}
*/