#pragma once

void Skybox_Init()
{
    glGenVertexArrays(1, &SkyboxVAO);
    glGenBuffers(1, &SkyboxVBO);
    glBindVertexArray(SkyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, SkyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxVertices), &SkyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    SkyboxTexture = Load_texture_environment(Path_Resources + "skybox/");

    SkyboxShader = Shader(Path_Shader + "skybox.vert", Path_Shader + "skybox.frag");
    SkyboxShader.use();
    SkyboxShader.setInt("skybox", 0);//设置纹理对应编号
}

void Open_skybox()
{
    if(Active_Skybox)
        Skybox_Init();
}

void Skybox_Draw()
{
    if (!Active_Skybox)
        return;
    //视图矩阵
    //-通过取4x4矩阵左上角的3x3矩阵来移除变换矩阵的位移部分
    //-将观察矩阵转换为3x3矩阵（移除位移），再将其转换回4x4矩阵，来达到类似的效果。
    glm::mat4 view_skybox = glm::mat4(glm::mat3(glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp)));
    //投影矩阵
    glm::mat4 projection_skybox = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //
    glm::mat4 model_skybox = glm::mat4(1.0f);
    model_skybox = glm::scale(model_skybox, vec3(99.0f));

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    //关闭深度写入
    glDepthMask(GL_FALSE);
    SkyboxShader.use();
    SkyboxShader.setMat4("view", view_skybox);
    SkyboxShader.setMat4("projection", projection_skybox);
    SkyboxShader.setMat4("model", model_skybox);
    glBindVertexArray(SkyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}