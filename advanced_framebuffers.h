#pragma once

//开启帧缓冲
void Open_framebuffers() 
{
    Active_Framebuffers = true;
    screenShader = Shader(Path_Shader + "framebuffers.vert", Path_Shader + "framebuffers.frag");

    Bloom();

    //设置着色器对应纹理图像的id
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
}

void Open_framebuffers_firstrender()
{
    // 帧缓冲第一处理阶段(Pass)
    //-------------------
    glBindFramebuffer(GL_FRAMEBUFFER, Bloom::fbo);//绑定自定义的帧缓冲对象//MSAAFramebuffer //HdrFBO
    glBindRenderbuffer(GL_RENDERBUFFER, Bloom::rbo);//绑定自定义的渲染缓冲对象//MSAARBO //RBODepth
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (Active_shadowMap)
    {
        //绑定阴影贴图进行渲染
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    }
}

void Open_framebuffers_secondrender()
{
    //泛光
    //----
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, HdrFBO);
    // 2.使用两次高斯模糊对明亮碎片进行模糊
    horizontal = true; first_iteration = true;
    GLuint amount = 10;
    //使用高斯模糊着色器
    Bloom::shaderBlur.use();
    for (unsigned int i = 0; i < amount; i++) {
        //绑定纹理对象，
        glBindFramebuffer(GL_FRAMEBUFFER, Bloom::pfbo[horizontal]);
        Bloom::shaderBlur.setInt("horizontal", horizontal);
        Bloom::shaderBlur.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? Bloom::cbo[1] : Bloom::pcbo[!horizontal]);//绑定其他帧缓冲区的纹理（如果是第一次迭代，则绑定场景中高光对象纹理）
        //渲染屏幕四边形
        RenderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    ////泛光
    //// ----
    //显示最终结果
    // -----------
    //3.现在将浮点颜色缓冲区渲染为2D四边形，并将色调映射HDR颜色渲染为默认帧缓冲区（钳制）的颜色范围
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_TEST);// | GL_DEPTH_BUFFER_BIT 
    //禁用深度测试，这样屏幕空间四元不会因为深度测试而被丢弃。
    glDisable(GL_DEPTH_TEST); 
    //使用屏幕图像着色器
    Bloom::shaderFinal.use();
    //Bloom::shaderFinal.setInt("screenTexture", 0);
    //Bloom::shaderFinal.setInt("bloomBlur", 1);
    //绑定相应图片
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

//渲染四边形到屏幕
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
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);//第二个参数设置的是纹理所拥有的样本个数。如果最后一个参数为GL_TRUE，图像将会对每个纹素使用相同的样本位置以及相同数量的子采样点个数
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
    //将纹理附加到帧缓冲区
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, t2, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t2, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1);//绑定其他帧缓冲区的纹理（如果是第一次迭代，则绑定场景中高光对象纹理）

    glBindTexture(GL_TEXTURE_2D, Bloom::pcbo[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //将纹理附加到帧缓冲区
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, Bloom::pcbo[2], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Bloom::pcbo[2], 0);
    //渲染屏幕四边形        
    RenderQuad();
}

void MSAA_Set()
{
    glGenFramebuffers(1, &HdrFBOFinal);
    glBindFramebuffer(GL_FRAMEBUFFER, HdrFBOFinal);
    glBindTexture(GL_TEXTURE_2D, TextureFinal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//这里使用FLOAT类型载入浮点缓冲区
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureFinal, 0);

    //多重采样纹理附件-离屏MSAA
    // ----------------
    //创建一个支持储存多个采样点的纹理
    glGenFramebuffers(1, &MSAAFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, MSAAFramebuffer);
    // 创建多采样颜色附着纹理
    //textureColorBufferMultiSampled[0] = generateMultiSampleTexture(Multisampling);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[0], 0);
    glGenTextures(2, textureColorBufferMultiSampled);
    for (int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[i]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Multisampling, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);//第二个参数设置的是纹理所拥有的样本个数。如果最后一个参数为GL_TRUE，图像将会对每个纹素使用相同的样本位置以及相同数量的子采样点个数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled[i], 0);
    }
    //为深度和模具附件创建renderbuffer对象
    glGenRenderbuffers(1, &MSAARBO);
    glBindRenderbuffer(GL_RENDERBUFFER, MSAARBO);
    //创建一个多重采样渲染缓冲对象,在指定（当前绑定的）渲染缓冲的内存存储时，将glRenderbufferStorage的调用改为glRenderbufferStorageMultisample
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, Multisampling, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAARBO);
    // 告诉OpenGL我们将使用（此帧缓冲区的）哪些颜色附件进行渲染
    //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    //第一次渲染为正常画布，第二次渲染为非正常画布(进行泛光操作)
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
    //检查创建好的帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Print::Line("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MSAA_Render()
{
    //离屏MSAA
    // --------
    // 将多重采样缓冲还原到中介FBO上
    glBindFramebuffer(GL_READ_FRAMEBUFFER, MSAAFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, HdrFBOFinal);//中介fbo
    ////缩小或者还原(Resolve)图像,够将一个帧缓冲中的某个区域复制到另一个帧缓冲(当前绑定帧缓冲)中，并且将多重采样缓冲还原
    //glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);//还原颜色缓冲//指定拉伸图像时要应用的插值
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);//还原深度缓冲//最近值还原
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_STENCIL_BUFFER_BIT, GL_NEAREST);//还原模板缓冲
}
