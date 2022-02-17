#version 330 core
layout (location = 0) out vec4 gPositionDepth;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

const float NEAR = 0.1f; // Projection matrix's near plane distance
const float FAR = 100.0f; // Projection matrix's far plane distance
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

void main()
{    
    // ����Ƭ�ε�λ��ʸ������һ��G��������
    gPositionDepth.xyz = FragPos;
    // ����������ȵ�gPositionDepth��alpha����
    gPositionDepth.a = LinearizeDepth(gl_FragCoord.z); 
    // ���淨����Ϣ��G����
    gNormal = normalize(Normal);
    // ����������ɫ
    gAlbedoSpec.rgb = vec3(0.95);
}