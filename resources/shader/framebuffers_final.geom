#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{    
    //���Ƴ�һ�����ӵĶ���
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex//gs_in[0]��Ϊֻ��һ�����붥��        //�����Ϊ�����м�
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   // 1:����
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right  //2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left      //3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right     // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top           // 5:����
    fColor = vec3(1.0, 1.0, 1.0);//������Ϊ����Ȥ������Ҳ���Լ�װ���Ƕ��죬�����һ���������ɫ����Ϊ��ɫ�����ݶ�����һЩѩ
    EmitVertex();
    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}