#pragma once
//光照是否开启
bool ActiveLight = true;
//点光源位置
glm::vec3 PointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};
//环境光照颜色/强度
glm::vec3 AmbientLight = glm::vec3(0.7f);

//光空间变换矩阵
mat4 LightSpaceMatrix = mat4(1.0f);