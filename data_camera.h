#pragma once

//摄像机位置
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
//摄像机方向向量
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//摄像机上向量
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//摄像机视野
float Fov = 40.0f;

//偏航角
float Yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a Yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
//俯仰角
float Pitch = 0.0f;

//是否第一次加载必要矩阵
bool FirstMat4 = true;
//透视矩阵
glm::mat4 Projection_Mat4 = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//视图矩阵
glm::mat4 View_Mat4 = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
//模型矩阵
glm::mat4 Model_Mat4 = glm::mat4(1.0f);