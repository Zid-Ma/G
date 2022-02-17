#pragma once
static GLboolean Active_diffuse = true;
static GLboolean Active_specular = true;
static GLboolean Active_envTexture = true;
static GLboolean Active_normalMap = false;
static GLboolean Active_shadowMap = true;
static GLboolean Active_ssao = true;

//Shader�еĿ���
static bool Active_shader = true;
static bool Active_shader_dirLight = true;
static bool Active_shader_pointLight = true;
static bool Active_shader_spotLight = true;

static bool Active_cull_face = true;
static bool Active_blend = true;
static bool Active_srgb = true;


//��һ֡����ǰ֡ʱ��
static float DeltaTime = 0.0f;
static float FixedDeltaTime = 0.0f;
//��һ֡��ʱ��
static float LastFrame = 0.0f;
static float FixedLastFrame = 0.0f;
//ʱ�������ÿ��һ������
static float TimeCout = 0.0f;
//ÿ��֡��
static int FramesPerSecond = 0;
//֡�ʼ���
static int FramesCount = 0;
//ÿ��֡������
static int FramesPerSecondX = 0;
//֡����ʾ
static bool FramesPerSecondDisplay = true;

//�����Ļʱ����ɫ
static vec4 SCR_Color = vec4(0.2f, 0.3f, 0.3f, 1.0f);