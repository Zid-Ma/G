#pragma once

// Framebuffer
// ----------------------------------------------------------------------------------------------------
//屏幕着色器
static Shader screenShader;
//水平方向高斯模糊, 第一次迭代
static bool horizontal = true, first_iteration = true;

//帧缓冲对象
//----------
static GLuint MSAAFramebuffer;//MSAA抗锯齿帧缓冲对象
static GLuint HdrFBOFinal;
//泛光_颜色缓冲帧缓冲对象
static GLuint TextureFinal;
static GLuint textureColorBufferMultiSampled[2];//MSAA抗锯齿纹理附件
//深度和模板附件
static GLuint MSAARBO;//MSAA抗锯齿用渲染缓冲对象

//开启帧缓冲
void Open_framebuffers();
//帧缓冲第一处理阶段
void Open_framebuffers_firstrender();
//帧缓冲第二处理阶段
void Open_framebuffers_secondrender();

//渲染屏幕正方形
void RenderQuad();

//设置延迟着色参数以及SSAO渲染参数
void SSAO_Shader_Set();

//设置离屏MSAA
void MSAA_Set();
//渲染离屏MSAA
void MSAA_Render();
//离屏缓冲纹理对象(多重采样数量)
GLuint generateMultiSampleTexture(GLuint samples);
//复制图像数据 1 to 2
void CopyTextureData(GLuint t1, GLuint t2);

//帧缓冲是否已开启
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
static glm::vec3 LightPos_SSAO;//灯光位置
static glm::vec3 LightColor_SSAO;//灯光颜色
static GLuint gBuffer_SSAO;//帧缓冲对象g
static GLuint gPositionDepth_SSAO, gNormal_SSAO, gAlbedo_SSAO;
static GLuint RboDepth_SSAO;//深度缓冲对象
static GLuint ssaoFBO, ssaoBlurFBO;//ssao帧缓冲对象，模糊ssao帧缓冲对象
static GLuint ssaoColorBuffer, ssaoColorBufferBlur;//颜色缓冲对象
static GLuint NoiseTexture_SSAO;//噪声纹理
static std::vector<glm::vec3> ssaoKernel;
static std::vector<glm::vec3> ssaoNoise;//噪声

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
//将合适的纹理路径按照立方体贴图枚举指定的顺序加载到一个vector中
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
