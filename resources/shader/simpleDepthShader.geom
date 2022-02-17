#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;//��������һ�������Σ�����ܹ�6�������Σ�6*3���㣬�����ܹ�18�����㣩

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    //������������ͼ��6����
    for(int face = 0; face < 6; ++face)
    {
        //ָ����Ⱦ���ĸ�������ñ���:gl_Layer// built-in variable that specifies to which face we render.
        gl_Layer = face; //�������Ǹ�������������ܿ���ÿ������ͼ�ν���Ⱦ����������ͼ����һ���档��Ȼ��ֻ�е���������һ�����ӵ������֡�������������ͼ�������Ч��
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            //������FragPos�������͸�������ɫ����������Ҫ����һ�����ֵ
            FragPos = gl_in[i].gl_Position;
            //��ÿ������ռ䶥��任����صĹ�ռ䣬����ÿ��������
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}