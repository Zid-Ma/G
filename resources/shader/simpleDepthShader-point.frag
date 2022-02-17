#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;//光的位置向量
uniform float far_plane;//视锥的远平面值

//计算自己的深度，这个深度就是每个fragment位置和光源位置之间的线性距离。计算自己的深度值使得之后的阴影计算更加直观。
void main()
{
    // get distance between fragment and light source//获取碎片和光源之间的距离
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane//映射到[0；1] 距离通过除以远_平面
    lightDistance = lightDistance / far_plane;

    // write this as modified depth//写入为fragment的深度值（modified/被改进的）
    gl_FragDepth = lightDistance;
}