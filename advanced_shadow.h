#pragma once
//��Ⱦ��Ӱ���ڴ�С
int SHADOW_WIDTH = 4096;
int SHADOW_HEIGHT = 4096;
//��Ⱦ��Ӱʱ��ƽ�����
GLfloat Near_plane = 1.0f;//����
//��Ⱦ��ӰʱԶƽ�����
GLfloat Far_plane = 107.5f;//Զ��
//������Ӱӳ��֡�������
GLuint DepthFBO_Shadow;
//������Ӱӳ����Ȼ�����ͼ����
GLuint DepthMapFBO_Shadow;

Shader SimpleDepthShader;

//����������Ӱӳ��
void Open_shadow();
//������Ӱӳ���һ����Ⱦ
void Open_shadow_firstrender_begin();
//����������Ӱӳ���һ����Ⱦ
void Open_shadow_firstrender_end();
//������Ӱӳ��ڶ�����Ⱦ
void Open_shadow_secondrender();
//������Ӱӳ����ɫ��ģ�;�������
void Open_Shadow_SetShaderMat4(const string& _name, const mat4& _mat);

void Open_shadow()
{

    Print::Debug("��Ӱӳ�� - ������Ӱӳ���������á���");

    //�����ͼ
    //---------
    //������Ӧ��ɫ��
    SimpleDepthShader = Shader(Path_Shader + "simpleDepthShader.vert", Path_Shader + "simpleDepthShader.frag");
    SimpleDepthShader.use();

    Print::Debug("��Ӱӳ��");

    //Ϊ��Ⱦ�������ͼ����һ��֡�������
    DepthFBO_Shadow;
    glGenFramebuffers(1, &DepthFBO_Shadow);
    //����һ��2D�����ṩ��֡�������Ȼ���ʹ��
    SHADOW_WIDTH = 1024; SHADOW_HEIGHT = 1024;//����ĸ߿�����Ϊ1024�����������ͼ�ķֱ���

    DepthMapFBO_Shadow;
    glGenTextures(1, &DepthMapFBO_Shadow);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);//��Ϊֻ�������ֵ,Ҫ�������ʽָ��ΪGL_DEPTH_COMPONENT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //���Ʒ�ʽ����ΪGL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);//GL_CLAMP_TO_BORDER//GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //�������������г��������ͼ���������ȷ�Χ��1.0���������������꽫��Զ������Ӱ֮�С����ǿ��Դ���һ���߿���ɫ��Ȼ��������ͼ��������ѡ������ΪGL_CLAMP_TO_BORDER��
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //�����ɵ����������Ϊ֡�������Ȼ���
    glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO_Shadow);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMapFBO_Shadow, 0);
    //��ʽ����OpenGL���ǲ������κ���ɫ���ݽ�����Ⱦ
    //glDrawBuffer(GL_NONE);//������Ҫ��ֻ���ڴӹ��͸��ͼ����Ⱦ������ʱ�������Ϣ��������ɫ����û���á�Ȼ������������ɫ�����֡��������ǲ������ģ�����������Ҫ��ʽ����OpenGL���ǲ������κ���ɫ���ݽ�����Ⱦ������ͨ��������glDrawBuffer��glReadBuffer�Ѷ��ͻ��ƻ�������ΪGL_NONE��������¡�
    //glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //�������ý����ֵ��Ⱦ�������֡��������ǾͿ��Կ�ʼ��һ���ˣ����������ͼ

    ////������Ӧλ������
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);//���ö�������0
}

void Open_shadow_firstrender_begin()
{
    //������Ȼ���
    glEnable(GL_DEPTH_TEST);
    //Ϊ���޸�peterƫ�ƣ�����ʹ�ñ����޳�
    //glCullFace(GL_FRONT);
    //RenderSceneToDepthMap();
    //glCullFace(GL_BACK); // ��Ҫ�������ԭ�ȵ�culling face

    // 1. ��ѡ��Ⱦ�����ͼ
    //--------------------
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);//һ��Ҫ�ǵõ���glViewport����Ϊ��Ӱ��ͼ����������ԭ����Ⱦ�ĳ�����ͨ���Ǵ��ڷֱ��ʣ����Ų�ͬ�ķֱ��ʣ�������Ҫ�ı��ӿڣ�viewport���Ĳ�������Ӧ��Ӱ��ͼ�ĳߴ�
    //����Ӧ֡����
    glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO_Shadow);
    //�����Ȼ���
    glClear(GL_DEPTH_BUFFER_BIT);
    //�ǵð������ͼ
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    //ConfigureShaderAndMatrices();
    int _magnification = 2;//3
    //��Ϊ��Դʹ������ͶӰ����͸��ͼ��û���κα���
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
    //����һ����ͼ�������任ÿ�����壬�����Ǳ任���ӹ�Դ�ӽǿɼ��Ŀռ���         //glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)//pointLightPositions[]
    glm::mat4 lightView = glm::lookAt(PlayerPos + (vec3(-7, 25, 13) * 2.0f), PlayerPos + (vec3(-7, 25, 13) * 2.0f) +  _front, _up); //glm::lookAt(glm::vec3(-1.0f, 3.0f, 0.0f), glm::vec3(-1.0f, 3.0f, 0.0f) + glm::vec3(-1.0, -1.0, 0.0), glm::vec3(0.0f, -1.0f, -1.0)); ;//glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //��������Ϊ�����ṩ��һ����ռ�ı任����
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    LightSpaceMatrix = lightSpaceMatrix;

    //��Ⱦ��Ȼ���
    SimpleDepthShader.use();
    //���ù�ռ�任����
    SimpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    //RenderScene(simpleDepthShader);//ʹ�������ͼ��ɫ����Ⱦ��ǰ����
#pragma region ��Ⱦ���������ͼ


#pragma endregion
}

void Open_shadow_firstrender_end()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //��Ⱦ�����ֵ����ʹ�������޳�
    glCullFace(GL_BACK);
}

void Open_shadow_secondrender()
{
    // 2. ������һ����Ⱦ�����������ʹ�������ͼ
    //------------------------------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //ConfigureShaderAndMatrices();
    //�ǵð������ͼ
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, DepthMapFBO_Shadow);
    //RenderScene();
}

void Open_Shadow_SetShaderMat4(const string &_name, const mat4 &_mat)
{
    SimpleDepthShader.use();
    SimpleDepthShader.setMat4(_name, _mat);
}