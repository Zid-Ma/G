#pragma once

//GLFW�������
static GLFWwindow* Window;

static bool WindowOnLoad = false;
static bool WindowClose = false;

//���ز�����
static int Multisampling = 2;

//������
static HDC hdc = (HDC)0x11D47;
static HGLRC hglrc = (HGLRC)0x10000;

//��ǰ������
static HWND HInstance;

//������
static GLuint SCR_WIDTH = 1920;// 1920
//����߶�
static GLuint SCR_HEIGHT = 1080;// 1080
//��������߿������Ļ���ĵľ���
static GLfloat SCR_WIDTH_CENTER_LEFT = -(SCR_WIDTH * 0.5f);
//����߶��±߿������Ļ���ĵľ���
static GLfloat SCR_HEIGHT_CENTER_TOP = (SCR_HEIGHT * 0.5f);
//�������ұ߿������Ļ���ĵľ���
static GLfloat SCR_WIDTH_CENTER_RIGHT = (SCR_WIDTH * 0.5f);
//����߶��ϱ߱߿������Ļ���ĵľ���
static GLfloat SCR_HEIGHT_CENTER_BOTTOM = -(SCR_HEIGHT * 0.5f);

void Window_Initialization()
{
	SCR_WIDTH_CENTER_LEFT = -(SCR_WIDTH * 0.5f);
	SCR_HEIGHT_CENTER_TOP = (SCR_HEIGHT * 0.5f);
	SCR_WIDTH_CENTER_RIGHT = (SCR_WIDTH * 0.5f);
	SCR_HEIGHT_CENTER_BOTTOM = -(SCR_HEIGHT * 0.5f);
}

//�Ƿ��һ�������봰��
bool FirstMouse = true;
//�����X�����һ֡λ��
float LastX = SCR_WIDTH / 2.0f;
//�����Y�����һ֡λ��
float LastY = SCR_HEIGHT / 2.0f;

#pragma region �ص�����
void framebuffer_size_callback(GLFWwindow* window, int widthCout, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//���λ�øı�
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
#pragma endregion