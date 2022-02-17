#pragma once

//����֡����
void Open_framebuffers() 
{
    Active_Framebuffers = true;
    screenShader = Shader(Path_Shader + "framebuffers.vert", Path_Shader + "framebuffers.frag");

    Bloom();

    //������ɫ����Ӧ����ͼ���id
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
}

void Open_framebuffers_firstrender()
{
    // ֡�����һ����׶�(Pass)
    //-------------------
    glBindFramebuffer(GL_FRAMEBUFFER, Bloom::fbo);//���Զ����֡�������//MSAAFramebuffer //HdrFBO
    glBindRenderbuffer(GL_RENDERBUFFER, Bloom::rbo);//���Զ������Ⱦ�������//MSAARBO //RBODepth
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �������ڲ�ʹ��ģ�建��
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (Active_shadowMap)
    {
        //����Ӱ��ͼ������Ⱦ
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    }
}

void Open_framebuffers_secondrender()
{
    //����
    //----
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, HdrFBO);
    // 2.ʹ�����θ�˹ģ����������Ƭ����ģ��
    horizontal = true; first_iteration = true;
    GLuint amount = 10;
    //ʹ�ø�˹ģ����ɫ��
    Bloom::shaderBlur.use();
    for (unsigned int i = 0; i < amount; i++) {
        //���������
        glBindFramebuffer(GL_FRAMEBUFFER, Bloom::pfbo[horizontal]);
        Bloom::shaderBlur.setInt("horizontal", horizontal);
        Bloom::shaderBlur.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? Bloom::cbo[1] : Bloom::pcbo[!horizontal]);//������֡����������������ǵ�һ�ε�������󶨳����и߹��������
        //��Ⱦ��Ļ�ı���
        RenderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    ////����
    //// ----
    //��ʾ���ս��
    // -----------
    //3.���ڽ�������ɫ��������ȾΪ2D�ı��Σ�����ɫ��ӳ��HDR��ɫ��ȾΪĬ��֡��������ǯ�ƣ�����ɫ��Χ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_TEST);// | GL_DEPTH_BUFFER_BIT 
    //������Ȳ��ԣ�������Ļ�ռ���Ԫ������Ϊ��Ȳ��Զ���������
    glDisable(GL_DEPTH_TEST); 
    //ʹ����Ļͼ����ɫ��
    Bloom::shaderFinal.use();
    //Bloom::shaderFinal.setInt("screenTexture", 0);
    //Bloom::shaderFinal.setInt("bloomBlur", 1);
    //����ӦͼƬ
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Bloom::cbo[0]);//TextureColorBuffer[0]//Bloom::cbo[0]//DepthMapFBO_Shadow
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Bloom::pcbo[!horizontal]);//pingpongColorBuffer[!horizontal]
    //SSAO_Shader_Set();

    RenderQuad();
    glEnable(GL_DEPTH_TEST);
}

static bool first_ui = true;
void Render_TextQuad()
{
    //glDisable(GL_DEPTH_TEST);
    if (first_ui)
    {
       ui = new UI();
       first_ui = false;
    }
    ui->Use();
    //ui->DrawTexts("XYKS!!! Hello Gamer!", -(SCR_WIDTH * 0.5f) + 50, (SCR_HEIGHT * 0.5f) - 50, 16);//SCR_WIDTH
    ui->DrawTexts("FPS:" + to_string(FramesPerSecond), (SCR_WIDTH * 0.5f) - 130, (SCR_HEIGHT * 0.5f) - 20, 16);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    //delete ui;

    //Quad::SetVAO();

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, ti);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glEnable(GL_DEPTH_TEST);
}

void Delete_TextQuad()
{
    try 
    {
        delete ui;
    }
    catch (exception e)
    {
        Print::Debug(e.what());
    }

}

//��Ⱦ�ı��ε���Ļ
void RenderQuad()
{
    Quad::SetVAO();
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SSAO_Shader_Set()
{
    int* _uniform_index = new int(2);
    *_uniform_index = 2;
    Bloom::shaderFinal.use();
    Bloom::shaderFinal.setInt("gPositionDepth", 0 + *_uniform_index);
    Bloom::shaderFinal.setInt("gNormal", 1 + *_uniform_index);
    Bloom::shaderFinal.setInt("gAlbedo", 2 + *_uniform_index);
    Bloom::shaderFinal.setInt("ssao", 3 + *_uniform_index);
    Bloom::shaderFinal.setBool("active_ssao", Active_ssao);
    glActiveTexture(GL_TEXTURE0 + *_uniform_index);
    glBindTexture(GL_TEXTURE_2D, gPositionDepth_SSAO);
    glActiveTexture(GL_TEXTURE1 + *_uniform_index);
    glBindTexture(GL_TEXTURE_2D, gNormal_SSAO);
    glActiveTexture(GL_TEXTURE2 + *_uniform_index);
    glBindTexture(GL_TEXTURE_2D, Bloom::cbo[0]);//Bloom::cbo[0] //gAlbedo_SSAO
    glActiveTexture(GL_TEXTURE3 + *_uniform_index);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    delete _uniform_index;
    // Also send light relevant uniforms
    glm::vec3 lightPosView = glm::vec3(View_Mat4 * glm::vec4(PointLightPositions[0], 1.0));//LightPos_SSAO  //PointLightPositions[0]
    Bloom::shaderFinal.setVec3("light.Position", lightPosView);
    Bloom::shaderFinal.setVec3("light.Color", LightColor_SSAO);
    // Update attenuation parameters
    const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
    const GLfloat linear = 0.09;
    const GLfloat quadratic = 0.032;
    Bloom::shaderFinal.setFloat("light.Linear", linear);
    Bloom::shaderFinal.setFloat("light.Quadratic", quadratic);
    Bloom::shaderFinal.setFloat("draw_mode", Draw_mode_SSAO);
}

GLuint generateMultiSampleTexture(GLuint samples)
{
    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);//�ڶ����������õ���������ӵ�е�����������������һ������ΪGL_TRUE��ͼ�񽫻��ÿ������ʹ����ͬ������λ���Լ���ͬ�������Ӳ��������
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    return texture;
}

void CopyTextureData(GLuint t1, GLuint t2)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Bloom::pfbo[2]);
    screenShader.use();
    glBindTexture(GL_TEXTURE_2D, t2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //�������ӵ�֡������
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, t2, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t2, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1);//������֡����������������ǵ�һ�ε�������󶨳����и߹��������

    glBindTexture(GL_TEXTURE_2D, Bloom::pcbo[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //�������ӵ�֡������
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, Bloom::pcbo[2], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Bloom::pcbo[2], 0);
    //��Ⱦ��Ļ�ı���        
    RenderQuad();
}

void MSAA_Set()
{
    glGenFramebuffers(1, &HdrFBOFinal);
    glBindFramebuffer(GL_FRAMEBUFFER, HdrFBOFinal);
    glBindTexture(GL_TEXTURE_2D, TextureFinal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//����ʹ��FLOAT�������븡�㻺����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureFinal, 0);

    //���ز���������-����MSAA
    // ----------------
    //����һ��֧�ִ����������������
    glGenFramebuffers(1, &MSAAFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, MSAAFramebuffer);
    // �����������ɫ��������
    //textureColorBufferMultiSampled[0] = generateMultiSampleTexture(Multisampling);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[0], 0);
    glGenTextures(2, textureColorBufferMultiSampled);
    for (int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[i]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Multisampling, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);//�ڶ����������õ���������ӵ�е�����������������һ������ΪGL_TRUE��ͼ�񽫻��ÿ������ʹ����ͬ������λ���Լ���ͬ�������Ӳ��������
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[i], 0);
    }
    //Ϊ��Ⱥ�ģ�߸�������renderbuffer����
    glGenRenderbuffers(1, &MSAARBO);
    glBindRenderbuffer(GL_RENDERBUFFER, MSAARBO);
    //����һ�����ز�����Ⱦ�������,��ָ������ǰ�󶨵ģ���Ⱦ������ڴ�洢ʱ����glRenderbufferStorage�ĵ��ø�ΪglRenderbufferStorageMultisample
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, Multisampling, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAARBO);
    // ����OpenGL���ǽ�ʹ�ã���֡�������ģ���Щ��ɫ����������Ⱦ
    //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    //��һ����ȾΪ�����������ڶ�����ȾΪ����������(���з������)
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    //��鴴���õ�֡�����Ƿ�����
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Print::Line("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAA_Render()
{
    //����MSAA
    // --------
    // �����ز������廹ԭ���н�FBO��
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSAAFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, HdrFBOFinal);//�н�fbo
    ////��С���߻�ԭ(Resolve)ͼ��,����һ��֡�����е�ĳ�������Ƶ���һ��֡����(��ǰ��֡����)�У����ҽ����ز������廹ԭ
    //glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);//��ԭ��ɫ����//ָ������ͼ��ʱҪӦ�õĲ�ֵ
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);//��ԭ��Ȼ���//���ֵ��ԭ
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_STENCIL_BUFFER_BIT, GL_NEAREST);//��ԭģ�建��
}
