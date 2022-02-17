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

#pragma region 提前定义函数
//获取应用程序路径
string get_path();
void get_gobal();
#pragma endregion

//------
//首次使用初始化
//------
void Initialization(void) {

    InitSteamApi();
    //全局变量初始化
    get_gobal();
    //Set = Settings();
    Set.Read();
    Set.Confirm();
    Window_Initialization();
    //SteamAPI_Init();
#pragma region GLFW初始化

    //GLFW初始化
    glfwInit();
    //配置GLFW，调试选项
    //OpenGL主版本号（GLFW_CONTEXT_VERSION_MAJOR）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//要设置的hint的名字（使用GLFW常量），值
    //OpenGL副版本号（GLFW_CONTEXT_VERSION_MINOR）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //OpenGL模式（GLFW_OPENGL_PROFILE）
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //窗口是否可调整大小（GLFW_RESIZABLE）
    glfwWindowHint(GLFW_RESIZABLE, TRUE);
    //窗口图标
    
#pragma region 多重采样
    //使用一个包含N个样本的多重采样缓冲 
    glfwWindowHint(GLFW_SAMPLES, Multisampling);//GLFW会自动创建一个每像素4个子采样点的深度和样本缓冲。这也意味着所有缓冲的大小都增长了4倍。
#pragma endregion

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#pragma endregion

#pragma region GLFW窗体对象创建

    //为窗体对象赋值
    Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My lying flat life", NULL, NULL);

    //如果窗体对象出现问题
    if (Window == NULL) {
        Print::Line("创建窗体对象时失败");
    }
    //设置窗口的上下文为当前线程的主上下文
    glfwMakeContextCurrent(Window);
    //设置window窗体不需要被关闭
    glfwSetWindowShouldClose(Window, false);

    //绑定相应回调函数
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(Window, mouse_button_callback);
    glfwSetCursorPosCallback(Window, mouse_callback);
    glfwSetScrollCallback(Window, scroll_callback);

    //设置鼠标在当前窗体的模式（鼠标隐藏且不改变位置）
    //glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#pragma endregion

#pragma region 设置OpenGL上下文

#pragma region 创建Windows窗体
    ////注册WNDCLASS类（结构体）
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

    ////创建窗体
    //HWND hWnd = CreateWindow((LPCWSTR)"OpenGL", (LPCWSTR)"Hello World.",
    //    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
    //    CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, GetModuleHandle(NULL), NULL);
    //if (!hWnd)
    //{
    //    cerr << "Create Window failed." << endl;
    //}

    //ShowWindow(hWnd, SW_SHOW);

#pragma endregion

#pragma region 销毁窗体
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


    ////设置一个新的窗口句柄
    //hdc = (HDC)0x11D47;
    //hglrc = (HGLRC)0x10000;
    ////将OpenGL上下文设置为当前线程的当前上下文
    //hdc = GetDC(hWnd);
    //printf("%p", hWnd);

    //填写PIXELFORMATDESCRIPTOR结构体
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

    ////选择合适的像素格式
    //// get the best available match of pixel format for the device context  
    //int  iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    //if (0 == iPixelFormat)
    //{
    //    cerr << "Choose pixel format failed." << endl;
    //}

    ////设置像素格式
    //// make that the pixel format of the device context  
    //if (!SetPixelFormat(hdc, iPixelFormat, &pfd))
    //{
    //    cerr << "Set pixel format failed." << endl;
    //}

    //wglMakeCurrent(hdc, wglCreateContext(hdc));//wglCreateContext(hdc)
    ////Window = (GLFWwindow*)x;//0x0055C148

    ////显示OpenGL渲染内容
    ////交换缓冲
    ////SwapBuffers(hdc);
#pragma endregion

#pragma region GLAD初始化

    //GLAD初始化,用来管理OpenGL函数指针的，调用任何OpenGL的函数之前都需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //返回初始化失败信息
        Print::Line("Failed to initialize GLAD - GLAD初始化失败");
        //return -1;
    }

#pragma endregion

#pragma region OpenGL初始化

    //设置OpenGL渲染窗体的尺寸大小，即视口(Viewport)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //设置OpenGL的全局状态（）
    //-----------------------------------
    if (true)
    {
        //启用深度测试
        glEnable(GL_DEPTH_TEST);
        Print::Line("启用深度测试");
        //指定“目标像素与当前像素在z方向上值大小比较”的函数，符合该函数关系的目标像素才进行绘制，否则对目标像素不予绘制。
        //glDepthFunc(GL_LESS);//GL_LESS,如果输入的深度值小于参考值，则通过
    }
    //模板缓冲
    //------------
    if (true)
    {
        //启用模板测试
        glEnable(GL_STENCIL_TEST);
        Print::Line("启用模板测试");
        //告诉OpenGL，只要一个片段的模板值等于(GL_EQUAL)参考值1，片段将会通过测试并被绘制，否则会被丢弃。
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //指定更新模板缓冲的时机与行为
        //sfail：模板测试失败时采取的行为。
        //dpfail：模板测试通过，但深度测试失败时采取的行为。
        //dppass：模板测试和深度测试都通过时采取的行为。
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }
    //混合
    //--------
    if (Active_blend)
    {
        //启用混合
        glEnable(GL_BLEND);
        Print::Line("启用混合");
        //设置源和目标因子
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //也可以使用glBlendFuncSeparate为RGB和alpha通道分别设置不同的选项：
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
        //OpenGL甚至给了我们更多的灵活性，允许我们改变方程中源和目标部分的运算符。当前源和目标是相加的，但如果愿意的话，我们也可以让它们相减。glBlendEquation(GLenum mode)允许我们设置运算符，它提供了三个选项：
        //GL_FUNC_ADD：默认选项，将两个分量相加：。
        //GL_FUNC_SUBTRACT：将两个分量相减： 。
        //GL_FUNC_REVERSE_SUBTRACT：将两个分量相减，但顺序相反：。
    }
    //面剔除
    //---------
    if (Active_cull_face)
    {
        //启用面剔除
        glEnable(GL_CULL_FACE);
        Print::Line("启用面剔除");
        //剔除背面
        glCullFace(GL_BACK);
        Print::Line("剔除背面");
        //顺时针为正向面(初始为CL_CWW逆时针为正向)
        //glFrontFace(GL_CW);
        //Print::Line("设置顺时针为正向面");
    }
    //点大小
    //------
    if (true)
    {
        //启用顶点着色器点大小修改
        glEnable(GL_PROGRAM_POINT_SIZE);
        Print::Line("启用顶点着色器点大小修改");
    }
    //抗锯齿
    // -------
    if (Multisampling > 0)
    {
        //启用多重采样
        glEnable(GL_MULTISAMPLE);
        Print::Line("启用多重采样");
        //启用抗锯齿
        glEnable(GL_BLEND);             //启用混合功能，将图形颜色同周围颜色相混合  
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_POLYGON_SMOOTH);     //多边形抗锯齿  
        glHint(GL_POLYGON_SMOOTH, GL_NICEST);    //GL_DONT_CARE 放弃，应该是系统默认吧//GL_FASTEST 速度优先//GL_NICEST 图形显示质量优先
        glEnable(GL_LINE_SMOOTH);        //线抗锯齿  
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable(GL_POINT_SMOOTH);       //点抗锯齿  
        glHint(GL_POINT_SMOOTH, GL_NICEST);
        Print::Line("启用抗锯齿");
    }
    //gamma校正
    //--------
    if (Active_srgb)
    {
        //启用sRGB颜色空间//OpenGL将自动执行gamma校正，包括默认帧缓冲
        glEnable(GL_FRAMEBUFFER_SRGB);
        Print::Line("启用sRGB颜色空间(将会自动执行gamma校正");
    }
#pragma endregion

#pragma region 读取OpenGL相关属性
    //OpenGL确保至少有16个包含4分量的顶点属性可用，但是有些硬件或许允许更多的顶点属性
    int nrAttributes = 0;
    //查询顶点属性的最大支持量
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    Print::Line("支持顶点属性的最大数量：" + to_string(nrAttributes));
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nrAttributes);
    Print::Line("支持uniform的最大数量：" + to_string(nrAttributes));

    Print::Line("XXX:" + to_string(2));
#pragma endregion

    //HInstance = GetActiveWindow();
    //InRealMain((LPSTR)"", (HINSTANCE)HInstance, SW_SHOW);//以原来的尺寸显示窗口
}

//------
//glfw：每当窗口大小改变（由操作系统或用户调整大小）时，都会执行此回调函数//glfw: whenever the Window size changed (by OS or user resize) this callback function executes
//------
void framebuffer_size_callback(GLFWwindow* window, int widthCout, int height)
{
    // make sure the viewport matches the new Window dimensions; note that widthCout and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, widthCout, height);
}

//获取当前应用程序路径
string get_path()
{
    char szFilePath[MAX_PATH + 1] = { 0 };
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    //获取最后一次出现该字符的位置，并将后续字符删除
    (strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
    string path = szFilePath;
    path += "\\";
    string finalpath = "";
    //替换相应字符
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
    Print::Line("当前应用程序路径：" + finalpath);
    return finalpath;
}
//设置应用程序路径
void set_path()
{
    Path = get_path();
    Path_Resources = Path + "resources/";
    Path_Shader = Path_Resources + "shader/";
    Path_Model = Path_Resources + "model/";
    Path_Text = Path_Resources + "text/";
    Path_Font = Path_Resources + "font/";
}
//设置碰撞体图层
void set_collider_layers()
{
    //Collider_Layers::Initialization_Collider_Layers(2);
    Collider_Layer* cl = new Collider_Layer();
    Collider_Layers::Collection.push_back(cl);
    Collider_Layers::Names.push_back("CultivatedLand");//耕地
    Collider_Layers::Names.push_back("");//
}
void get_gobal()
{
    Active_shader = true;
    Active_shader_dirLight = true;
    Active_shader_pointLight = true;
    Active_shader_spotLight = true;
    //应用程序路径
    set_path();
    //碰撞体图层
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