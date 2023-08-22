#pragma once
#include <random>

OBJMesh* NanoMan;
void Open_ssao()
{
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    //创建着色器
    shaderGeometryPass_SSAO = Shader(Path_Shader + "ssao_geometry.vert", Path_Shader + "ssao_geometry.frag");
    shaderLightingPass_SSAO = Shader(Path_Shader + "ssao.vert", Path_Shader + "ssao_lighting.frag");
    shaderSSAO_SSAO = Shader(Path_Shader + "ssao.vert", Path_Shader + "ssao.frag");
    shaderSSAOBlur_SSAO = Shader(Path_Shader + "ssao.vert", Path_Shader + "ssao_blur.frag");
    int _texture_index = 0;//纹理对象地址偏移
    //设置纹理值
    shaderLightingPass_SSAO.use();
    shaderLightingPass_SSAO.setInt("gPositionDepth", 0);
    shaderLightingPass_SSAO.setInt("gNormal", 1);
    shaderLightingPass_SSAO.setInt("gAlbedo", 2);
    shaderLightingPass_SSAO.setInt("ssao", 3);
    shaderSSAO_SSAO.use();
    shaderSSAO_SSAO.setInt("gPositionDepth", 0);
    shaderSSAO_SSAO.setInt("gNormal", 1);
    shaderSSAO_SSAO.setInt("texNoise", 2);
    //加载模型对象
    //nanosuit = Model();
    //灯光位置和颜色
    LightPos_SSAO = glm::vec3(2.0, 4.0, -2.0);
    LightColor_SSAO = glm::vec3(0.2, 0.2, 0.7);
    // Set up G-Buffer
    // 3 textures:
    // 1. Positions + depth (RGBA)
    // 2. Color (RGB) 
    // 3. Normals (RGB) 
    //创建帧缓冲对象g
        glGenFramebuffers(1, &gBuffer_SSAO);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer_SSAO);
    //设置深度值颜色缓冲，法线颜色缓冲，
    //// - Position + linear depth color buffer
    //glGenTextures(1, &gPositionDepth_SSAO);
    //glBindTexture(GL_TEXTURE_2D, gPositionDepth_SSAO);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth_SSAO, 0);
    //// - Normal color buffer
    //glGenTextures(1, &gNormal_SSAO);
    //glBindTexture(GL_TEXTURE_2D, gNormal_SSAO);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal_SSAO, 0);
    // - Albedo color buffer
    glGenTextures(1, &gAlbedo_SSAO);
    glBindTexture(GL_TEXTURE_2D, gAlbedo_SSAO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo_SSAO, 0);
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    //分别为创建好的3各颜色缓冲设置3张画布

        GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);


    // - Create and attach depth buffer (renderbuffer)
    //创建深度缓冲

        glGenRenderbuffers(1, &RboDepth_SSAO);
        glBindRenderbuffer(GL_RENDERBUFFER, RboDepth_SSAO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RboDepth_SSAO);
        // - Finally check if framebuffer is complete
        //检查深度缓冲对象是否创建成功
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print::Debug("GBuffer Framebuffer not complete!");
        //glBindRenderbuffer(GL_RENDERBUFFER, 0);



    // Also create framebuffer to hold SSAO processing stage 
    //创建ssao帧缓冲对象，和模糊ssao帧缓冲对象
    glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    // - SSAO color buffer
    //ssao颜色缓冲
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Print::Debug("SSAO Framebuffer not complete!");
    // - and blur stage
    //模糊ssao颜色缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Print::Debug("SSAO Blur Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Sample kernel
    //ssao半圆核心
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;

        // Scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // Noise texture
    //噪声贴图
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &NoiseTexture_SSAO);
    glBindTexture(GL_TEXTURE_2D, NoiseTexture_SSAO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //NanoMan = new OBJMesh("F:/Resources/C++/Hello Under World-Cube/Debug/Mode/nanosuit/nanosuit.obj", false);
}

glm::mat4 ssao_model;
void Open_ssao_firstrender()
{
    if (!Active_ssao)
    {
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    // 1. Geometry Pass: render scene's geometry/color data into gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer_SSAO);
    glBindRenderbuffer(GL_RENDERBUFFER, RboDepth_SSAO);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
    //glm::mat4 _projection = glm::perspective(CameraZoom, (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 50.0f);
    //glm::mat4 _view = camera.GetViewMatrix();
    shaderGeometryPass_SSAO.use();
    shaderGeometryPass_SSAO.setMat4("projection", Projection_Mat4);
    shaderGeometryPass_SSAO.setMat4("view", View_Mat4);
    // Floor cube
    ssao_model = glm::mat4();
    ssao_model = glm::translate(ssao_model, glm::vec3(0.0, 3.0f, 0.0f));
    //ssao_model = glm::scale(ssao_model, glm::vec3(20.0f, 1.0f, 20.0f));
    shaderGeometryPass_SSAO.setMat4("model", ssao_model);
    RenderCube();
    // Nanosuit model on the floor
    ssao_model = glm::mat4();
    ssao_model = glm::translate(ssao_model, glm::vec3(0.0f, 0.0f, 5.0));
    ssao_model = glm::rotate(ssao_model, -90.0f, glm::vec3(1.0, 0.0, 0.0));
    ssao_model = glm::scale(ssao_model, glm::vec3(0.5f));
    //shaderGeometryPass_SSAO.setMat4("model", ssao_model);
    //nanosuit.Draw(shaderGeometryPass_SSAO);
    //NanoMan->Draw_SSAO(shaderGeometryPass_SSAO);
}

void Open_ssao_secondrender()
{
    if (!Active_ssao)
    {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Create SSAO texture
    //创建ssao贴图
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
    shaderSSAO_SSAO.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPositionDepth_SSAO);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal_SSAO);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, NoiseTexture_SSAO);
    // Send kernel + rotation 
    for (GLuint i = 0; i < 64; ++i)
        shaderSSAO_SSAO.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    shaderSSAO_SSAO.setMat4("projection", Projection_Mat4);
    RenderQuad();
}

void Open_ssao_threadrender()
{
    if (!Active_ssao)
    {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 3. Blur SSAO texture to remove noise
    //模糊ssao贴图去除噪声
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
    shaderSSAOBlur_SSAO.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    RenderQuad();
    glClearColor(SCR_Color.r, SCR_Color.g, SCR_Color.b, SCR_Color.a);
    glEnable(GL_DEPTH_TEST);
}

void Open_ssao_fourthrender()
{
    if (!Active_ssao)
    {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // 4. Lighting Pass: traditional deferred Blinn-Phong lighting now with added screen-space ambient occlusion
    //去除ssao明亮部分
    glDisable(GL_DEPTH_TEST); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderLightingPass_SSAO.use();
    int _texture_index = 0;//纹理对象地址偏移
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPositionDepth_SSAO);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal_SSAO);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedo_SSAO);
    glActiveTexture(GL_TEXTURE3); // Add extra SSAO texture to lighting pass
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);//ssaoColorBuffer //ssaoColorBufferBlur
    // Also send light relevant uniforms
    glm::vec3 lightPosView = glm::vec3(View_Mat4 * glm::vec4(LightPos_SSAO, 1.0));
    shaderLightingPass_SSAO.setVec3("light.Position", lightPosView);
    shaderLightingPass_SSAO.setVec3("light.Color", LightColor_SSAO);
    // Update attenuation parameters
    const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
    const GLfloat linear = 0.09;
    const GLfloat quadratic = 0.032;
    shaderLightingPass_SSAO.setFloat("light.Linear", linear);
    shaderLightingPass_SSAO.setFloat("light.Quadratic", quadratic);
    shaderLightingPass_SSAO.setFloat("draw_mode", Draw_mode_SSAO);
    RenderQuad();

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


GLfloat lerp(GLfloat a, GLfloat b, GLfloat NihaoWanjia)
{
    return a + NihaoWanjia * (b - a);
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO_ssao = 0;
GLuint quadVBO_ssao;
void RenderQuad_ssao()
{
    if (quadVAO_ssao == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO_ssao);
        glGenBuffers(1, &quadVBO_ssao);
        glBindVertexArray(quadVAO_ssao);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO_ssao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO_ssao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO_ssao = 0;
GLuint cubeVBO_ssao = 0;
void RenderCube()
{
    // Initialize (if necessary)
    if (cubeVAO_ssao == 0)
    {
        GLfloat vertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO_ssao);
        glGenBuffers(1, &cubeVBO_ssao);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_ssao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO_ssao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO_ssao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

//半球核心
void SSAOKernel()
{
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;

        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);

        //随机转动核心
        std::vector<glm::vec3> ssaoNoise;
        for (GLuint i = 0; i < 16; i++)
        {
            glm::vec3 noise(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f);
            ssaoNoise.push_back(noise);
        }
        //随机旋转向量的4x4纹理
        GLuint NoiseTexture_SSAO;
        glGenTextures(1, &NoiseTexture_SSAO);
        glBindTexture(GL_TEXTURE_2D, NoiseTexture_SSAO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //帧缓冲对象
        GLuint ssaoFBO;
        glGenFramebuffers(1, &ssaoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        GLuint ssaoColorBuffer;
        //颜色缓冲对象
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//由于环境遮蔽的结果是一个灰度值，只需要纹理的红色分量，所以将颜色缓冲的内部格式设置为GL_RED。
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

        // 几何处理阶段: 渲染到G缓冲中

        // 使用G缓冲渲染SSAO纹理

        // 光照处理阶段: 渲染场景光照
    }
}
