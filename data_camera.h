#pragma once

//�����λ��
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
//�������������
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//�����������
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//�������Ұ
float Fov = 40.0f;

//ƫ����
float Yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a Yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//������
float Pitch = 0.0f;

//�Ƿ��һ�μ��ر�Ҫ����
bool FirstMat4 = true;
//͸�Ӿ���
glm::mat4 Projection_Mat4 = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//��ͼ����
glm::mat4 View_Mat4 = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
//ģ�;���
glm::mat4 Model_Mat4 = glm::mat4(1.0f);