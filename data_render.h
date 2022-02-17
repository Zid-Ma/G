#pragma once
static GLboolean Active_diffuse = true;
static GLboolean Active_specular = true;
static GLboolean Active_envTexture = true;
static GLboolean Active_normalMap = false;
static GLboolean Active_shadowMap = true;
static GLboolean Active_ssao = true;

//Shader中的开关
static bool Active_shader = true;
static bool Active_shader_dirLight = true;
static bool Active_shader_pointLight = true;
static bool Active_shader_spotLight = true;

static bool Active_cull_face = true;
static bool Active_blend = true;
static bool Active_srgb = true;


//上一帧到当前帧时间
static float DeltaTime = 0.0f;
static float FixedDeltaTime = 0.0f;
//上一帧的时间
static float LastFrame = 0.0f;
static float FixedLastFrame = 0.0f;
//时间计数，每过一秒清零
static float TimeCout = 0.0f;
//每秒帧率
static int FramesPerSecond = 0;
//帧率计数
static int FramesCount = 0;
//每秒帧率限制
static int FramesPerSecondX = 0;
//帧率显示
static bool FramesPerSecondDisplay = true;

//清除屏幕时的颜色
static vec4 SCR_Color = vec4(0.2f, 0.3f, 0.3f, 1.0f);