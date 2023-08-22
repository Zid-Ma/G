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
    SkyboxShader.setInt("skybox", 0);//���������Ӧ���
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
    //��ͼ����
    //-ͨ��ȡ4x4�������Ͻǵ�3x3�������Ƴ��任�����λ�Ʋ���
    //-���۲����ת��Ϊ3x3�����Ƴ�λ�ƣ����ٽ���ת����4x4�������ﵽ���Ƶ�Ч����
    glm::mat4 view_skybox = glm::mat4(glm::mat3(glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp)));
    //ͶӰ����
    glm::mat4 projection_skybox = glm::perspective(glm::radians(Fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //
    glm::mat4 model_skybox = glm::mat4(1.0f);
    model_skybox = glm::scale(model_skybox, vec3(99.0f));

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_DEPTH_TEST);
    //�ر����д��
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