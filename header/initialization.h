#pragma once
//#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"SOIL.lib")
#include <glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "multiplatform.h"
#include "operator.h"
#include "language.h"

#include "class_math.h"
#include "class_math_character.h"

#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h" /* http://nothings.org/stb/stb_image_write.h */
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb-master/stb_truetype.h> /* http://nothings.org/stb/stb_truetype.h */

//#include "header/mesh_me.h"
//#include "header/mode_me.h"
#include "shader_me.h"

#include <gl.h>
using namespace std;
using namespace glm;

#include "class_collider.h"
#include "class_collider_layer.h"
#include "check_collision.h"
#include "class_ray.h"

#include "data_path.h"
#include "data_window.h"
#include "data_render.h"
#include "data_shader.h"
#include "data_light.h"
#include "data_layer.h"

#include "data_game.h"
#include "data_camera.h"
#include "data_player.h"
#include "data_mode_me.h"
#include "data_definition.h"
#include "advanced_data.h"

#include "data_set.h"
#include "function_event.h"

#include "../C++/G/sdk/public/steam/steam_api.h"
//#include "../C++/G/sdk/\steamworksexample/Main.cpp"
#include "steam.h"

#pragma region ��ǰ���庯��
//��ȡӦ�ó���·��
string get_path();
void get_gobal();
#pragma endregion

//------
//�״�ʹ�ó�ʼ��
//------
void Initialization(void) {

    InitSteamApi();
    //ȫ�ֱ�����ʼ��
    get_gobal();
    //Set = Settings();
    Set.Read();
    Set.Confirm();
    Window_Initialization();
    //SteamAPI_Init();
#pragma region GLFW��ʼ��

    //GLFW��ʼ��
    glfwInit();
    //����GLFW������ѡ��
    //OpenGL���汾�ţ�GLFW_CONTEXT_VERSION_MAJOR��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Ҫ���õ�hint�����֣�ʹ��GLFW��������ֵ
    //OpenGL���汾�ţ�GLFW_CONTEXT_VERSION_MINOR��
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //OpenGLģʽ��GLFW_OPENGL_PROFILE��
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //�����Ƿ�ɵ�����С��GLFW_RESIZABLE��
    glfwWindowHint(GLFW_RESIZABLE, TRUE);
    //����ͼ��
    
#pragma region ���ز���
    //ʹ��һ������N�������Ķ��ز������� 
    glfwWindowHint(GLFW_SAMPLES, Multisampling);//GLFW���Զ�����һ��ÿ����4���Ӳ��������Ⱥ��������塣��Ҳ��ζ�����л���Ĵ�С��������4����
#pragma endregion

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#pragma endregion

#pragma region GLFW������󴴽�

    //Ϊ�������ֵ
    Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My lying flat life", NULL, NULL);

    //�����������������
    if (Window == NULL) {
        Print::Line("�����������ʱʧ��");
    }
    //���ô��ڵ�������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(Window);
    //����window���岻��Ҫ���ر�
    glfwSetWindowShouldClose(Window, false);

    //����Ӧ�ص�����
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(Window, mouse_button_callback);
    glfwSetCursorPosCallback(Window, mouse_callback);
    glfwSetScrollCallback(Window, scroll_callback);

    //��������ڵ�ǰ�����ģʽ����������Ҳ��ı�λ�ã�
    //glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma endregion

#pragma region ����OpenGL������

#pragma region ����Windows����
    ////ע��WNDCLASS�ࣨ�ṹ�壩
    //WNDCLASS    wc; // Windows Class Structure  

    //HINSTANCE hInstance = GetModuleHandle(NULL);  // Grab An Instance For Our Window  
    //// Redraw On Size, And Own DC For Window.  
    //wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    //wc.lpfnWndProc = (WNDPROC)Window; // WndProc Handles Messages  
    //wc.cbClsExtra = 0;                    // No Extra Window Data  
    //wc.cbWndExtra = 0;                    // No Extra Window Data  
    //wc.hInstance = hInstance;           // Set The Instance  
    //wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);    // Load_Model The Default Icon  
    //wc.hCursor = LoadCursor(NULL, IDC_ARROW);   // Load_Model The Arrow Pointer  
    //wc.hbrBackground = NULL;                     // No Background Required For GL  
    //wc.lpszMenuName = NULL;                     // We Don't Want A Menu  
    //wc.lpszClassName = (LPCWSTR)"OpenGL";                 // Set The Class Name  

    //if (!RegisterClass(&wc)) // Attempt To Register The Window Class  
    //{
    //    cout << "Failed to register the Window." << endl;
    //}

    ////��������
    //HWND hWnd = CreateWindow((LPCWSTR)"OpenGL", (LPCWSTR)"Hello World.",
    //    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
    //    CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, GetModuleHandle(NULL), NULL);
    //if (!hWnd)
    //{
    //    cerr << "Create Window failed." << endl;
    //}

    //ShowWindow(hWnd, SW_SHOW);

#pragma endregion

#pragma region ���ٴ���
    //if (hdc && !ReleaseDC(hWnd, hdc)) // Are We Able To Release The DC  
    //{
    //    cerr << "Release Device Context Failed." << endl;
    //    hdc = NULL; // Set DC To NULL  
    //}

    //if (hWnd && !DestroyWindow(hWnd)) // Are We Able To Destroy The Window?  
    //{
    //    cerr << "Could Not Release hWnd." << endl;
    //    hWnd = NULL; // Set hWnd To NULL  
    //}

    //if (!UnregisterClass((LPCWSTR)"OpenGL", hInstance))   // Are We Able To Unregister Class  
    //{
    //    cerr << "Could Not Unregister Class." << endl;
    //    hInstance = NULL; // Set hInstance To NULL  
    //}
#pragma endregion


    ////����һ���µĴ��ھ��
    //hdc = (HDC)0x11D47;
    //hglrc = (HGLRC)0x10000;
    ////��OpenGL����������Ϊ��ǰ�̵߳ĵ�ǰ������
    //hdc = GetDC(hWnd);
    //printf("%p", hWnd);

    //��дPIXELFORMATDESCRIPTOR�ṹ��
    //static PIXELFORMATDESCRIPTOR pfd = {
    //sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor  
    //1,                              // Version Number  
    //PFD_DRAW_TO_WINDOW |            // Format Must Support Window  
    //PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL  
    //PFD_DOUBLEBUFFER,               // Must Support Double Buffering  
    //PFD_TYPE_RGBA,                  // Request An RGBA Format  
    //32,                           // Select Our Color Depth  
    //0, 0, 0, 0, 0, 0,               // Color Bits Ignored  
    //0,                              // No Alpha Buffer  
    //0,                              // Shift Bit Ignored  
    //0,                              // No Accumulation Buffer  
    //0, 0, 0, 0,                     // Accumulation Bits Ignored  
    //16,                             // 16Bit Z-Buffer (Depth Buffer)    
    //0,                              // No Stencil Buffer  
    //0,                              // No Auxiliary Buffer  
    //PFD_MAIN_PLANE,                 // Main Drawing Layer  
    //0,                              // Reserved  
    //0, 0, 0                         // Layer Masks Ignored  
    //};

    ////ѡ����ʵ����ظ�ʽ
    //// get the best available match of pixel format for the device context  
    //int  iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    //if (0 == iPixelFormat)
    //{
    //    cerr << "Choose pixel format failed." << endl;
    //}

    ////�������ظ�ʽ
    //// make that the pixel format of the device context  
    //if (!SetPixelFormat(hdc, iPixelFormat, &pfd))
    //{
    //    cerr << "Set pixel format failed." << endl;
    //}

    //wglMakeCurrent(hdc, wglCreateContext(hdc));//wglCreateContext(hdc)
    ////Window = (GLFWwindow*)x;//0x0055C148

    ////��ʾOpenGL��Ⱦ����
    ////��������
    ////SwapBuffers(hdc);
#pragma endregion

#pragma region GLAD��ʼ��

    //GLAD��ʼ��,��������OpenGL����ָ��ģ������κ�OpenGL�ĺ���֮ǰ����Ҫ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //���س�ʼ��ʧ����Ϣ
        Print::Line("Failed to initialize GLAD - GLAD��ʼ��ʧ��");
        //return -1;
    }

#pragma endregion

#pragma region OpenGL��ʼ��

    //����OpenGL��Ⱦ����ĳߴ��С�����ӿ�(Viewport)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //����OpenGL��ȫ��״̬����
    //-----------------------------------
    if (true)
    {
        //������Ȳ���
        glEnable(GL_DEPTH_TEST);
        Print::Line("������Ȳ���");
        //ָ����Ŀ�������뵱ǰ������z������ֵ��С�Ƚϡ��ĺ��������ϸú�����ϵ��Ŀ�����زŽ��л��ƣ������Ŀ�����ز�����ơ�
        //glDepthFunc(GL_LESS);//GL_LESS,�����������ֵС�ڲο�ֵ����ͨ��
    }
    //ģ�建��
    //------------
    if (true)
    {
        //����ģ�����
        glEnable(GL_STENCIL_TEST);
        Print::Line("����ģ�����");
        //����OpenGL��ֻҪһ��Ƭ�ε�ģ��ֵ����(GL_EQUAL)�ο�ֵ1��Ƭ�ν���ͨ�����Բ������ƣ�����ᱻ������
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //ָ������ģ�建���ʱ������Ϊ
        //sfail��ģ�����ʧ��ʱ��ȡ����Ϊ��
        //dpfail��ģ�����ͨ��������Ȳ���ʧ��ʱ��ȡ����Ϊ��
        //dppass��ģ����Ժ���Ȳ��Զ�ͨ��ʱ��ȡ����Ϊ��
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }
    //���
    //--------
    if (Active_blend)
    {
        //���û��
        glEnable(GL_BLEND);
        Print::Line("���û��");
        //����Դ��Ŀ������
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //Ҳ����ʹ��glBlendFuncSeparateΪRGB��alphaͨ���ֱ����ò�ͬ��ѡ�
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        //OpenGL�����������Ǹ��������ԣ��������Ǹı䷽����Դ��Ŀ�겿�ֵ����������ǰԴ��Ŀ������ӵģ������Ը��Ļ�������Ҳ���������������glBlendEquation(GLenum mode)����������������������ṩ������ѡ�
        //GL_FUNC_ADD��Ĭ��ѡ�������������ӣ���
        //GL_FUNC_SUBTRACT����������������� ��
        //GL_FUNC_REVERSE_SUBTRACT�������������������˳���෴����
    }
    //���޳�
    //---------
    if (Active_cull_face)
    {
        //�������޳�
        glEnable(GL_CULL_FACE);
        Print::Line("�������޳�");
        //�޳�����
        glCullFace(GL_BACK);
        Print::Line("�޳�����");
        //˳ʱ��Ϊ������(��ʼΪCL_CWW��ʱ��Ϊ����)
        //glFrontFace(GL_CW);
        //Print::Line("����˳ʱ��Ϊ������");
    }
    //���С
    //------
    if (true)
    {
        //���ö�����ɫ�����С�޸�
        glEnable(GL_PROGRAM_POINT_SIZE);
        Print::Line("���ö�����ɫ�����С�޸�");
    }
    //�����
    // -------
    if (Multisampling > 0)
    {
        //���ö��ز���
        glEnable(GL_MULTISAMPLE);
        Print::Line("���ö��ز���");
        //���ÿ����
        glEnable(GL_BLEND);             //���û�Ϲ��ܣ���ͼ����ɫͬ��Χ��ɫ����  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POLYGON_SMOOTH);     //����ο����  
        glHint(GL_POLYGON_SMOOTH, GL_NICEST);    //GL_DONT_CARE ������Ӧ����ϵͳĬ�ϰ�//GL_FASTEST �ٶ�����//GL_NICEST ͼ����ʾ��������
        glEnable(GL_LINE_SMOOTH);        //�߿����  
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POINT_SMOOTH);       //�㿹���  
        glHint(GL_POINT_SMOOTH, GL_NICEST);
        Print::Line("���ÿ����");
    }
    //gammaУ��
    //--------
    if (Active_srgb)
    {
        //����sRGB��ɫ�ռ�//OpenGL���Զ�ִ��gammaУ��������Ĭ��֡����
        glEnable(GL_FRAMEBUFFER_SRGB);
        Print::Line("����sRGB��ɫ�ռ�(�����Զ�ִ��gammaУ��");
    }
#pragma endregion

#pragma region ��ȡOpenGL�������
    //OpenGLȷ��������16������4�����Ķ������Կ��ã�������ЩӲ�������������Ķ�������
    int nrAttributes = 0;
    //��ѯ�������Ե����֧����
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    Print::Line("֧�ֶ������Ե����������" + to_string(nrAttributes));
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nrAttributes);
    Print::Line("֧��uniform�����������" + to_string(nrAttributes));

    Print::Line("XXX:" + to_string(2));
#pragma endregion

    //HInstance = GetActiveWindow();
    //InRealMain((LPSTR)"", (HINSTANCE)HInstance, SW_SHOW);//��ԭ���ĳߴ���ʾ����
}

//------
//glfw��ÿ�����ڴ�С�ı䣨�ɲ���ϵͳ���û�������С��ʱ������ִ�д˻ص�����//glfw: whenever the Window size changed (by OS or user resize) this callback function executes
//------
void framebuffer_size_callback(GLFWwindow* window, int widthCout, int height)
{
    // make sure the viewport matches the new Window dimensions; note that widthCout and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, widthCout, height);
}

//��ȡ��ǰӦ�ó���·��
string get_path()
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    //��ȡ���һ�γ��ָ��ַ���λ�ã����������ַ�ɾ��
    (strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�  
    string path = szFilePath;
    path += "\\";
    string finalpath = "";
    //�滻��Ӧ�ַ�
    for (int i = 0; i < path.length(); i++)
    {
        if (path.substr(i, 1) == "\\")
        {
            finalpath.append("/");
        }
        else
        {
            finalpath.append(path.substr(i, 1));
        }
    }
    Print::Line("��ǰӦ�ó���·����" + finalpath);
    return finalpath;
}
//����Ӧ�ó���·��
void set_path()
{
    Path = get_path();
    Path_Resources = Path + "resources/";
    Path_Shader = Path_Resources + "shader/";
    Path_Model = Path_Resources + "model/";
    Path_Text = Path_Resources + "text/";
    Path_Font = Path_Resources + "font/";
}
//������ײ��ͼ��
void set_collider_layers()
{
    //Collider_Layers::Initialization_Collider_Layers(2);
    Collider_Layer* cl = new Collider_Layer();
    Collider_Layers::Collection.push_back(cl);
    Collider_Layers::Names.push_back("CultivatedLand");//����
    Collider_Layers::Names.push_back("");//
}
void get_gobal()
{
    Active_shader = true;
    Active_shader_dirLight = true;
    Active_shader_pointLight = true;
    Active_shader_spotLight = true;
    //Ӧ�ó���·��
    set_path();
    //��ײ��ͼ��
    set_collider_layers();
}


#include "set_vertices.h"
#include "set_texture.h"
#include "set_loop.h"
#include "set_draw.h"
#include "set_shader.h"
#include "set_matrix.h"


#include "advanced_shadow.h"

#include "class_text.h"
#include "class_quad.h"
#include "class_square.h"
#include "class_cube.h"
#include "class_user_interface.h"

#include "class_model_obj.h"
#include "class_model.h"
#include "class_modelx.h"

#include "class_crops.h"

#include "advanced_framebuffers.h"
#include "advanced_ScreenSpaceAmbientOcclusion.h"

#include "class_scene.h"
#include "data_scene.h"
#include "scene_first.h"

#include "event_mouse.h"
#include "check_input.h"
#include "fixed_update.h"