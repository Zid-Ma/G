#pragma once

// Framebuffer
// ----------------------------------------------------------------------------------------------------
//��Ļ��ɫ��
static Shader screenShader;
//ˮƽ�����˹ģ��, ��һ�ε���
static bool horizontal = true, first_iteration = true;

//֡�������
//----------
static GLuint MSAAFramebuffer;//MSAA�����֡�������
static GLuint HdrFBOFinal;
//����_��ɫ����֡�������
static GLuint TextureFinal;
static GLuint textureColorBufferMultiSampled[2];//MSAA�����������
//��Ⱥ�ģ�帽��
static GLuint MSAARBO;//MSAA���������Ⱦ�������

//����֡����
void Open_framebuffers();
//֡�����һ����׶�
void Open_framebuffers_firstrender();
//֡����ڶ�����׶�
void Open_framebuffers_secondrender();

//��Ⱦ��Ļ������
void RenderQuad();

//�����ӳ���ɫ�����Լ�SSAO��Ⱦ����
void SSAO_Shader_Set();

//��������MSAA
void MSAA_Set();
//��Ⱦ����MSAA
void MSAA_Render();
//���������������(���ز�������)
GLuint generateMultiSampleTexture(GLuint samples);
//����ͼ������ 1 to 2
void CopyTextureData(GLuint t1, GLuint t2);

//֡�����Ƿ��ѿ���
static bool Active_Framebuffers = false;

#pragma region Shadow
// Shadow
// ----------------------------------------------------------------------------------------------------
static bool Active_Shadow = true;
#pragma endregion


// SSAO
// ----------------------------------------------------------------------------------------------------
// Function prototypes
void RenderCube();
void RenderQuad();
GLfloat lerp(GLfloat a, GLfloat b, GLfloat NihaoWanjia);
// Options
static GLuint Draw_mode_SSAO = 1;
static Shader shaderGeometryPass_SSAO;
static Shader shaderLightingPass_SSAO;
static Shader shaderSSAO_SSAO;
static Shader shaderSSAOBlur_SSAO;
//Model nanosuit;
static glm::vec3 LightPos_SSAO;//�ƹ�λ��
static glm::vec3 LightColor_SSAO;//�ƹ���ɫ
static GLuint gBuffer_SSAO;//֡�������g
static GLuint gPositionDepth_SSAO, gNormal_SSAO, gAlbedo_SSAO;
static GLuint RboDepth_SSAO;//��Ȼ������
static GLuint ssaoFBO, ssaoBlurFBO;//ssao֡�������ģ��ssao֡�������
static GLuint ssaoColorBuffer, ssaoColorBufferBlur;//��ɫ�������
static GLuint NoiseTexture_SSAO;//��������
static std::vector<glm::vec3> ssaoKernel;
static std::vector<glm::vec3> ssaoNoise;//����

#pragma region Skybox
// Skybox
// ----------------------------------------------------------------------------------------------------
static float SkyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
//�����ʵ�����·��������������ͼö��ָ����˳����ص�һ��vector��
static vector<std::string> SkyboxFaces
{
    Path_Resources + "skybox/right.jpg",
    Path_Resources + "skybox/left.jpg",
    Path_Resources + "skybox/top.jpg",
    Path_Resources + "skybox/bottom.jpg",
    Path_Resources + "skybox/front.jpg",
    Path_Resources + "skybox/back.jpg"
};
static GLuint SkyboxVAO, SkyboxVBO;
static GLuint SkyboxTexture;
static Shader SkyboxShader;
static bool Active_Skybox = true;
#pragma endregion
