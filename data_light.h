#pragma once
//�����Ƿ���
bool ActiveLight = true;
//���Դλ��
glm::vec3 PointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};
//����������ɫ/ǿ��
glm::vec3 AmbientLight = glm::vec3(0.7f);

//��ռ�任����
mat4 LightSpaceMatrix = mat4(1.0f);