#pragma once

//GLFW窗体对象
static GLFWwindow* Window;

static bool WindowOnLoad = false;
static bool WindowClose = false;

//多重采样率
static int Multisampling = 2;

//窗体句柄
static HDC hdc = (HDC)0x11D47;
static HGLRC hglrc = (HGLRC)0x10000;

//当前窗体句柄
static HWND HInstance;

//窗体宽度
static GLuint SCR_WIDTH = 1920;// 1920
//窗体高度
static GLuint SCR_HEIGHT = 1080;// 1080
//窗体宽度左边框基于屏幕中心的距离
static GLfloat SCR_WIDTH_CENTER_LEFT = -(SCR_WIDTH * 0.5f);
//窗体高度下边框基于屏幕中心的距离
static GLfloat SCR_HEIGHT_CENTER_TOP = (SCR_HEIGHT * 0.5f);
//窗体宽度右边框基于屏幕中心的距离
static GLfloat SCR_WIDTH_CENTER_RIGHT = (SCR_WIDTH * 0.5f);
//窗体高度上边边框基于屏幕中心的距离
static GLfloat SCR_HEIGHT_CENTER_BOTTOM = -(SCR_HEIGHT * 0.5f);

void Window_Initialization()
{
	SCR_WIDTH_CENTER_LEFT = -(SCR_WIDTH * 0.5f);
	SCR_HEIGHT_CENTER_TOP = (SCR_HEIGHT * 0.5f);
	SCR_WIDTH_CENTER_RIGHT = (SCR_WIDTH * 0.5f);
	SCR_HEIGHT_CENTER_BOTTOM = -(SCR_HEIGHT * 0.5f);
}

//是否第一次鼠标进入窗体
bool FirstMouse = true;
//鼠标在X轴的上一帧位置
float LastX = SCR_WIDTH / 2.0f;
//鼠标在Y轴的上一帧位置
float LastY = SCR_HEIGHT / 2.0f;

#pragma region 回调函数
void framebuffer_size_callback(GLFWwindow* window, int widthCout, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//鼠标位置改变
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//鼠标滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
#pragma endregion