#pragma once

//(着色器，漫反射贴图)
void Load_shader_d(Shader& shader, GLuint* Texture1);
//(着色器，漫反射贴图，镜面反射贴图，环境映射贴图)
void Load_shader_d(Shader& shader, GLuint* Texture1, GLuint* Texture2, GLuint* Texture3);
//使用着色器
static void Load_shader_u(Shader& shader, bool active_true);
//设置着色器全局函数代码
static void Load_shader_goball(Shader& shader);

void Load_shader(Shader& shader)
{
    Load_shader_d(shader, 0, 0, 0);
}

static void Load_shader_goball(Shader& shader)
{
    if (Active_shader == true)
    {
    }
    shader.setBool("active.dirLight", Active_shader_dirLight);
    shader.setBool("active.pointLight", Active_shader_pointLight);
    shader.setBool("active.spotLight", Active_shader_spotLight);

    shader.setVec3("viewPos", CameraPos);
    shader.setVec3("spotLight.position", CameraPos);
    shader.setVec3("spotLight.direction", CameraFront);

    shader.setVec3("pointLights[0].position", PointLightPositions[0]);//点光源位置
    shader.setVec3("pointLights[1].position", PointLightPositions[1]);
    shader.setVec3("pointLights[2].position", PointLightPositions[2]);
    shader.setVec3("pointLights[3].position", PointLightPositions[3]);
}

static bool Shader_True = true;
static void Load_shader_u(Shader& shader, bool active_true)
{
    Load_shader_goball(shader);
}

//(着色器，漫反射贴图，镜面反射贴图，环境映射贴图)
static void Load_shader_d(Shader &shader, GLuint *Texture1, GLuint *Texture2, GLuint * Texture3)
{
    shader.use();
    if (Shader_True)
    {

    }
    shader.setInt("material.diffuse", 0);//*Texture1
    shader.setInt("material.specular", 1);
    shader.setInt("material.envTexture", 2);
    shader.setInt("material.normalMap", 3);
    shader.setInt("material.shadowMap", 4);
    shader.setInt("material.gPositionDepth", 5);
    shader.setInt("material.gNormal", 6);
    shader.setInt("material.gAlbedo", 7);
    shader.setInt("material.ssao", 8);
    shader.setBool("material.active_diffuse", Active_diffuse);
    shader.setBool("material.active_specular", Active_specular);
    shader.setBool("material.active_envTexture", Active_envTexture);
    shader.setBool("material.active_normalMap", Active_normalMap);
    shader.setBool("material.active_shadowMap", Active_shadowMap);
    shader.setBool("material.active_shadowMap", Active_ssao);

    shader.setBool("active.dirLight", Active_shader_dirLight);
    shader.setBool("active.pointLight", Active_shader_pointLight);
    shader.setBool("active.spotLight", Active_shader_spotLight);

    shader.setVec3("viewPos", CameraPos);
    shader.setFloat("material.shininess", 32.0f);

    // directional light
    shader.setVec3("dirLight.direction", 10.2f, 10.0f, 10.3f);//定向光位置 -0.2f, -1.0f, -0.3f
    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shader.setVec3("pointLights[0].position", PointLightPositions[0]);//点光源位置
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09);
    shader.setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    shader.setVec3("pointLights[1].position", PointLightPositions[1]);
    shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[1].constant", 1.0f);
    shader.setFloat("pointLights[1].linear", 0.09);
    shader.setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    shader.setVec3("pointLights[2].position", PointLightPositions[2]);
    shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[2].constant", 1.0f);
    shader.setFloat("pointLights[2].linear", 0.09);
    shader.setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    shader.setVec3("pointLights[3].position", PointLightPositions[3]);
    shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[3].constant", 1.0f);
    shader.setFloat("pointLights[3].linear", 0.09);
    shader.setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    shader.setVec3("spotLight.position", CameraPos);
    shader.setVec3("spotLight.direction", CameraFront);
    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09);
    shader.setFloat("spotLight.quadratic", 0.032);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}