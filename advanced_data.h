#pragma once

// Framebuffer
// ----------------------------------------------------------------------------------------------------
//��Ļ��ɫ��
Shader screenShader;
//ˮƽ�����˹ģ��, ��һ�ε���
bool horizontal = true, first_iteration = true;

//֡�������
//----------
GLuint MSAAFramebuffer;//MSAA�����֡�������
GLuint HdrFBOFinal;
//����_��ɫ����֡�������
GLuint TextureFinal;
GLuint textureColorBufferMultiSampled[2];//MSAA�����������
//��Ⱥ�ģ�帽��
GLuint MSAARBO;//MSAA���������Ⱦ�������

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

// Shadow
// ----------------------------------------------------------------------------------------------------
static bool Active_Shadow = true;

// SSAO
// ----------------------------------------------------------------------------------------------------
// Function prototypes
void RenderCube();
void RenderQuad();
GLfloat lerp(GLfloat a, GLfloat b, GLfloat NihaoWanjia);
// Options
GLuint Draw_mode_SSAO = 1;
Shader shaderGeometryPass_SSAO;
Shader shaderLightingPass_SSAO;
Shader shaderSSAO_SSAO;
Shader shaderSSAOBlur_SSAO;
//Model nanosuit;
glm::vec3 LightPos_SSAO;//�ƹ�λ��
glm::vec3 LightColor_SSAO;//�ƹ���ɫ
GLuint gBuffer_SSAO;//֡�������g
GLuint gPositionDepth_SSAO, gNormal_SSAO, gAlbedo_SSAO;
GLuint RboDepth_SSAO;//��Ȼ������
GLuint ssaoFBO, ssaoBlurFBO;//ssao֡�������ģ��ssao֡�������
GLuint ssaoColorBuffer, ssaoColorBufferBlur;//��ɫ�������
GLuint NoiseTexture_SSAO;//��������
std::vector<glm::vec3> ssaoKernel;
std::vector<glm::vec3> ssaoNoise;//����