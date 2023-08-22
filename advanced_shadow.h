#pragma once
//渲染阴影窗口大小
int SHADOW_WIDTH = 4096;
int SHADOW_HEIGHT = 4096;
//渲染阴影时近平面距离
GLfloat Near_plane = 1.0f;//近景
//渲染阴影时远平面距离
GLfloat Far_plane = 107.5f;//远景
//定向阴影映射帧缓冲对象
GLuint DepthFBO_Shadow;
//定向阴影映射深度缓冲贴图对象
GLuint DepthMapFBO_Shadow;

Shader SimpleDepthShader;

//开启定向阴影映射
void Open_shadow();
//定向阴影映射第一次渲染
void Open_shadow_firstrender_begin();
//结束定向阴影映射第一次渲染
void Open_shadow_firstrender_end();
//定向阴影映射第二次渲染
void Open_shadow_secondrender();
//定向阴影映射着色器模型矩阵设置
void Open_Shadow_SetShaderMat4(const string& _name, const mat4& _mat);

void Open_shadow()
{

    Print::Debug("阴影映射 - 定向阴影映射正在启用……");

    //深度贴图
    //---------
    //加载相应着色器
    SimpleDepthShader = Shader(Path_Shader + "simpleDepthShader.vert", Path_Shader + "simpleDepthShader.frag");
    SimpleDepthShader.use();

    Print::Debug("阴影映射");

    //为渲染的深度贴图创建一个帧缓冲对象
    DepthFBO_Shadow;
    glGenFramebuffers(1, &DepthFBO_Shadow);
    //创建一个2D纹理，提供给帧缓冲的深度缓冲使用
    SHADOW_WIDTH = 1024; SHADOW_HEIGHT = 1024;//纹理的高宽设置为1024：这是深度贴图的分辨率

    DepthMapFBO_Shadow;
    glGenTextures(1, &DepthMapFBO_Shadow);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);//因为只关心深度值,要把纹理格式指定为GL_DEPTH_COMPONENT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //环绕方式设置为GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_BORDER//GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //我们宁可让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中。我们可以储存一个边框颜色，然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER：
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //把生成的深度纹理作为帧缓冲的深度缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO_Shadow);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMapFBO_Shadow, 0);
    //显式告诉OpenGL我们不适用任何颜色数据进行渲染
    //glDrawBuffer(GL_NONE);//我们需要的只是在从光的透视图下渲染场景的时候深度信息，所以颜色缓冲没有用。然而，不包含颜色缓冲的帧缓冲对象是不完整的，所以我们需要显式告诉OpenGL我们不适用任何颜色数据进行渲染。我们通过将调用glDrawBuffer和glReadBuffer把读和绘制缓冲设置为GL_NONE来做这件事。
    //glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //合理配置将深度值渲染到纹理的帧缓冲后，我们就可以开始第一步了：生成深度贴图

    ////配置相应位置属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);//启用顶点属性0
}

void Open_shadow_firstrender_begin()
{
    //启用深度缓冲
    glEnable(GL_DEPTH_TEST);
    //为了修复peter偏移，这里使用背面剔除
    //glCullFace(GL_FRONT);
    //RenderSceneToDepthMap();
    //glCullFace(GL_BACK); // 不要忘记设回原先的culling face

    // 1. 首选渲染深度贴图
    //--------------------
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//一定要记得调用glViewport。因为阴影贴图经常和我们原来渲染的场景（通常是窗口分辨率）有着不同的分辨率，我们需要改变视口（viewport）的参数以适应阴影贴图的尺寸
    //绑定相应帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO_Shadow);
    //清除深度缓冲
    glClear(GL_DEPTH_BUFFER_BIT);
    //记得绑定深度贴图
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    //ConfigureShaderAndMatrices();
    int _magnification = 2;//3
    //将为光源使用正交投影矩阵，透视图将没有任何变形
    glm::mat4 lightProjection = glm::ortho(-10.0f * _magnification, 10.0f * _magnification, -10.0f * _magnification, 10.0f * _magnification, Near_plane, Far_plane);
    glm::vec3 _front, _up;
    float _yaw = -60.0f, _pitch = -60.0f;
    _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front.y = sin(glm::radians(_pitch));
    _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _yaw = -60.0f; _pitch = -60.0f + 90.0f;
    _up.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _up.y = sin(glm::radians(_pitch));
    _up.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    //创建一个视图矩阵来变换每个物体，把它们变换到从光源视角可见的空间中         //glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)//pointLightPositions[]
    glm::mat4 lightView = glm::lookAt(PlayerPos + (vec3(-7, 25, 13) * 2.0f), PlayerPos + (vec3(-7, 25, 13) * 2.0f) +  _front, _up); //glm::lookAt(glm::vec3(-1.0f, 3.0f, 0.0f), glm::vec3(-1.0f, 3.0f, 0.0f) + glm::vec3(-1.0, -1.0, 0.0), glm::vec3(0.0f, -1.0f, -1.0)); ;//glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //二者相结合为我们提供了一个光空间的变换矩阵
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    LightSpaceMatrix = lightSpaceMatrix;

    //渲染深度缓冲
    SimpleDepthShader.use();
    //设置光空间变换矩阵
    SimpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //RenderScene(simpleDepthShader);//使用深度贴图着色器渲染当前场景
#pragma region 渲染场景深度贴图


#pragma endregion
}

void Open_shadow_firstrender_end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //渲染完深度值后再使用正面剔除
    glCullFace(GL_BACK);
}

void Open_shadow_secondrender()
{
    // 2. 像往常一样渲染场景，但这次使用深度贴图
    //------------------------------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //ConfigureShaderAndMatrices();
    //记得绑定深度贴图
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    //RenderScene();
}

void Open_Shadow_SetShaderMat4(const string &_name, const mat4 &_mat)
{
    SimpleDepthShader.use();
    SimpleDepthShader.setMat4(_name, _mat);
}