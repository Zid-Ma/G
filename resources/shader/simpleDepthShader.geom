#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;//我们输入一个三角形，输出总共6个三角形（6*3顶点，所以总共18个顶点）

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    //遍历立方体贴图的6个面
    for(int face = 0; face < 6; ++face)
    {
        //指定渲染到哪个面的内置变量:gl_Layer// built-in variable that specifies to which face we render.
        gl_Layer = face; //但当我们更新这个变量就能控制每个基本图形将渲染到立方体贴图的哪一个面。当然这只有当我们有了一个附加到激活的帧缓冲的立方体贴图纹理才有效：
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            //将最后的FragPos变量发送给像素着色器，我们需要计算一个深度值
            FragPos = gl_in[i].gl_Position;
            //将每个世界空间顶点变换到相关的光空间，生成每个三角形
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}