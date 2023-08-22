#pragma once
void Process_Event_Mouse(vec3 _direction);
void Process_Event_MouseRight(vec3 _direction);
void Process_UIEvent_Mouse(vec3 _click_point);
void Process_UIPress_Mouse(vec3 _press_point);
void Process_Down_F(GLFWwindow* window);
void Process_Down_R(GLFWwindow* window);
void Process_Down_T(GLFWwindow* window);
void Process_Down_SPACE(GLFWwindow* window);
void Process_Down_Number(GLFWwindow* window);

float LastInput = 0.0f, DeltaInput = 0.0f;
int Cursor_Input_Value = 1;//输入模式 3：摄像机自由移动；2：摄像机跟随人物移动；1：摄像机跟随人物进行2D移动(鼠标出现)
bool T_Release = true;//按T进行输入模式的切换
vec3 LastPlayerPos;
bool PlayerToEarth = false;
bool Ctrl_Release = true;
//鼠标左键按下冷却
static float MouseButtonLeftDownDeltaTime = 0.0f;
//------
//处理所有输入：查询GLFW是否在此帧中按下/释放相关键并做出相应反应//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//------
void processInput(GLFWwindow* window)
{
    // 每帧时间逻辑(per-frame time logic)
    //------------------------------------
    float current = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || WindowClose)
        glfwSetWindowShouldClose(window, true);
    //按键T
    Process_Down_T(window);

    if (LastPlayerPos.y == PlayerPos.y)
    {
        PlayerToEarth = true;
    }
    else
    {
        PlayerToEarth = false;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Ctrl_Release = false;
    }
    else
    {
        Ctrl_Release = true;
    }

    float cameraSpeed = 5.5 * FixedDeltaTime;
    if (Cursor_Input_Value == 3)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            CameraPos += cameraSpeed * CameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            CameraPos -= cameraSpeed * CameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * cameraSpeed;
            Process_Down_F(window);
            Process_Down_R(window);
    }
    else if (Cursor_Input_Value == 2)
    {
        PlayerSpeed = 3.0 * FixedDeltaTime;
        if (PlayerPos == LastPlayerPos)
        {
            
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            PlayerPos += PlayerSpeed * PlayerFront;
            //CameraPos += PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            PlayerPos -= PlayerSpeed * PlayerFront;
            //CameraPos -= PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            PlayerPos -= glm::normalize(glm::cross(PlayerFront, PlayerUp)) * PlayerSpeed;
            //CameraPos -= glm::normalize(glm::cross(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f))) * PlayerSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            PlayerPos += glm::normalize(glm::cross(PlayerFront, PlayerUp)) * PlayerSpeed;
            //CameraPos += glm::normalize(glm::cross(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f))) * PlayerSpeed;
        }
        Process_Down_SPACE(window);
        PlayerJumpMinX();
        Process_Down_F(window);
        Process_Down_R(window);
        Process_Down_Number(window);
        //获取摄像机位置
        CameraPos = PlayerPos - (CameraFront * 0.0f) + vec3(0.0f, 1.0f, 0.0f);
    }
    else
    {
        PlayerSpeed = 1.0 * FixedDeltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            PlayerPos += PlayerSpeed * PlayerFront;
            //CameraPos += PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            PlayerPos -= PlayerSpeed * PlayerFront;
            //CameraPos -= PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            PlayerPos -= glm::normalize(glm::cross(PlayerFront, PlayerUp)) * PlayerSpeed;
            //CameraPos -= glm::normalize(glm::cross(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f))) * PlayerSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            PlayerPos += glm::normalize(glm::cross(PlayerFront, PlayerUp)) * PlayerSpeed;
            //CameraPos += glm::normalize(glm::cross(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f))) * PlayerSpeed;
        }
        Process_Down_SPACE(window);
        Process_Down_R(window);
    }
    LastPlayerPos = PlayerPos;
}

//实时更新
void Process_RealTimeChange()
{
    //鼠标按下冷却
    if (MouseButtonLeftDownDeltaTime > 0.0f)
    {
        MouseButtonLeftDownDeltaTime += DeltaTime;
        if (MouseButtonLeftDownDeltaTime > 1.0f)
            MouseButtonLeftDownDeltaTime = 0.0f;
    }
}

//鼠标的位置位置自屏幕左上方往右下为正
static vec3 Cursor_Point = vec3(0.0f);
static vec3 Last_cursor_point = vec3(0.0f);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    vec3 cursor_point_screenCenter = vec3(Cursor_Point.x - (SCR_WIDTH/2), -(Cursor_Point.y - (SCR_HEIGHT / 2)), Cursor_Point.z);
    //鼠标点击检测
    if (Cursor_Input_Value == 1)
    {
        //Process_UIEvent_Mouse(glfwSetCursor(window, GLFW_KEY_LEFT) == GLFW_PRESS);
        if (action == GLFW_PRESS) switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //"Mosue left button clicked!"
            if (MouseButtonLeftDownDeltaTime == 0.0f || true)
            {
                MouseButtonLeftDownDeltaTime = 0.1f;
                Process_UIPress_Mouse(cursor_point_screenCenter);
                Last_cursor_point = cursor_point_screenCenter;
            }
            //Print::Debug("鼠标按下时基于屏幕中心点的位置：" + to_string(cursor_point_screenCenter.x)+ "???" + to_string(cursor_point_screenCenter.y));
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            //"Mosue middle button clicked!"
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            //"Mosue right button clicked!"
            break;
        default:
            return;
        }
        else if (action == GLFW_RELEASE) switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //"Mosue left button clicked!"
            if (distance(Last_cursor_point, cursor_point_screenCenter) < 10.0f)
            {
                Process_UIEvent_Mouse(cursor_point_screenCenter);
                Last_cursor_point = vec3(0.0f);
            }
            //Print::Debug("鼠标按下时基于屏幕中心点的位置：" + to_string(cursor_point_screenCenter.x)+ "???" + to_string(cursor_point_screenCenter.y));
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            //"Mosue middle button clicked!"
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            //"Mosue right button clicked!"
            break;
        default:
            return;
        }
    }
    else if (Cursor_Input_Value == 2)
    {
        //Process_UIEvent_Mouse(glfwSetCursor(window, GLFW_KEY_LEFT) == GLFW_PRESS);
        if (action == GLFW_PRESS) switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            //"Mosue left button clicked!"
            Process_Event_Mouse(CameraFront);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            //"Mosue middle button clicked!"
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            //"Mosue right button clicked!"
            Process_Event_MouseRight(CameraFront);
            break;
        default:
            return;
        }
    }
    return;
}

//------
//glfw：只要鼠标移动，就会调用此回调//glfw: whenever the mouse moves, this callback is called
//------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Cursor_Input_Value == 3)
    {
        if (FirstMouse)
        {
            LastX = xpos;
            LastY = ypos;
            FirstMouse = false;
        }

        float xoffset = xpos - LastX;
        float yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top
        LastX = xpos;
        LastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when Pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        CameraFront = glm::normalize(front);
        glm::vec3 front_p;
        front_p.x = cos(glm::radians(Yaw)) * cos(glm::radians(0.0f));
        front_p.y = sin(glm::radians(0.0f));
        front_p.z = sin(glm::radians(Yaw)) * cos(glm::radians(0.0f));
        PlayerFront = glm::normalize(front_p);
    }
    else if (Cursor_Input_Value == 2)
    {
        if (FirstMouse)
        {
            LastX = xpos;
            LastY = ypos;
            FirstMouse = false;
        }

        float xoffset = xpos - LastX;
        float yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top
        LastX = xpos;
        LastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when Pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        CameraFront = glm::normalize(front);
        glm::vec3 front_p;
        front_p.x = cos(glm::radians(Yaw)) * cos(glm::radians(0.0f));
        front_p.y = sin(glm::radians(0.0f));
        front_p.z = sin(glm::radians(Yaw)) * cos(glm::radians(0.0f));
        PlayerFront = glm::normalize(front_p);
    }
    else if (Cursor_Input_Value == 1)
    {
        Cursor_Point.r = xpos;
        Cursor_Point.y = ypos;
    }
}

//------
//glfw：每当鼠标滚轮滚动时，调用此回调//glfw: whenever the mouse scroll wheel scrolls, this callback is called
//------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (Ctrl_Release == true)
    {
        void* scene_ptr = (Scene_First*)scene->Get_scene(1);
        //物品切换
        if (scene_ptr != NULL)
        {
            Scene_First* s = (Scene_First*)scene_ptr;
            int length = s->items_length;
            int cur_i = s->items_collection[s->items_current_i];
            s->fonts[cur_i].mod = 1;
            int count = 0;
            if(yoffset > 0)
                count = (-(int)yoffset + s->items_current_i + (length * (int)yoffset)) % length;
            else
                count = (-(int)yoffset + s->items_current_i + (length * -(int)yoffset)) % length;
            if (count >= 0 && count < 6)
                s->items_current_i = count;
            else
                Print::Debug(to_string(count));
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;

            if (yoffset > 0)
                s->fonts[15].mod--;
            else
                s->fonts[15].mod++;
            if (s->fonts[15].mod > 4)
                s->fonts[15].mod = -1;
            else if(s->fonts[15].mod < -1)
                s->fonts[15].mod = 4;

            s->fonts[15].mod = count - 1;
        }
    }
    else
    {
        Fov -= (float)yoffset;
        if (Fov < 1.0f)
            Fov = 1.0f;
        if (Fov > 45.0f)
            Fov = 45.0f;
    }
}

void AspotLight()
{
    Active_shader = true;
    if (Active_shader_spotLight)
    {
        //关闭聚光
        Active_shader_spotLight = false;
        Active_shader_dirLight = true;
        Active_shader_pointLight = true;
        Print::Debug(to_string(Active_shader_dirLight));
    }
    else
    {
        //打开聚光
        Active_shader_spotLight = true;
        Active_shader_dirLight = true;
        Active_shader_pointLight = true;
        Print::Debug(to_string(Active_shader_spotLight));
    }
}
static vector<int> Hover_Last_ui_font_count;
static vector<UI_Font*> Hover_Last_ui_font;
void Process_UIHover_Mouse(vec3 _hover_point)
{
    UI_Font* current_font = NULL;
    //Print::Line("???" + to_string(Hover_Mouse.size()));
    //鼠标悬停时改变字体颜色
    for (int i = 0; i < Hover_Mouse.size(); i++)
    {
        if (Hover_Mouse[i].CheckEvent(_hover_point, "hover"))
        {
            current_font = Hover_Mouse[i].ui_Font;
            if (Hover_Mouse[i].Function != NULL)
                //Print::Debug("剩下的");
                Hover_Mouse[i].Function();
            bool _last = false;
            for (int iv = 0; iv < Hover_Last_ui_font.size(); iv++)
            {
                if (Hover_Last_ui_font[iv] == Hover_Mouse[i].ui_Font)
                {
                    _last = true;
                }
            }
            if (!_last)
            {
                Hover_Last_ui_font.push_back(Hover_Mouse[i].ui_Font);
                Hover_Last_ui_font_count.push_back(0);
                Hover_Mouse[i].ui_Font->color = vec4(0.1f, 0.3f, 0.8f, 1.0f);
            }
            break;
        }
    }
    if (Hover_Last_ui_font.size() > 0)
    {
        //重置字体颜色
        for (int i = 0; i < Hover_Last_ui_font.size(); i++)
        {
            //时间计数
            Hover_Last_ui_font_count[i]++;
            if (Hover_Last_ui_font_count[i] > 3 && Hover_Last_ui_font[i] != current_font)
            {
                Hover_Last_ui_font[i]->color = vec4(0.0f);
                Hover_Last_ui_font[i] = NULL;
                Hover_Last_ui_font_count[i] = 0;
                Hover_Last_ui_font.erase(Hover_Last_ui_font.begin() + i);
                Hover_Last_ui_font_count.erase(Hover_Last_ui_font_count.begin() + i);
            }
        }
    }
}
static vector<int> Press_Last_ui_font_count;
static vector<UI_Font*> Press_Last_ui_font;
void Process_UIPress_Mouse(vec3 _press_point)
{
    UI_Font* current_font = NULL;
    //Print::Line("???" + to_string(Hover_Mouse.size()));
    //鼠标按下时改变字体颜色
    for (int i = 0; i < Press_Mouse.size(); i++)
    {
        if (Press_Mouse[i].CheckEvent(_press_point, "press"))
        {
            current_font = Press_Mouse[i].ui_Font;
            //假如函数不为空，则调用函数
            if (Press_Mouse[i].Function != NULL)
                //Print::Debug("剩下的");
                Press_Mouse[i].Function();
            bool _last = false;
            for (int iv = 0; iv < Press_Last_ui_font.size(); iv++)
            {
                //当找到已激活改变颜色的字体时
                if (Press_Last_ui_font[iv] == Press_Mouse[i].ui_Font)
                {
                    _last = true;
                }
            }
            //改变字体颜色
            if (!_last)
            {
                Press_Last_ui_font.push_back(Press_Mouse[i].ui_Font);
                Press_Last_ui_font_count.push_back(0);
                Press_Mouse[i].ui_Font->color = vec4(0.1f, 0.3f, 0.8f, 1.0f);
            }
            break;
        }
    }
    if (Press_Last_ui_font.size() > 0)
    {
        for (int i = 0; i < Press_Last_ui_font.size(); i++)
        {
            Press_Last_ui_font_count[i]++;
            if (Press_Last_ui_font_count[i] > 5 && Press_Last_ui_font[i] != current_font)
            {
                Press_Last_ui_font[i]->color = vec4(0.0f);
                Press_Last_ui_font[i] = NULL;
                Press_Last_ui_font_count[i] = 0;
                Press_Last_ui_font.erase(Press_Last_ui_font.begin() + i);
                Press_Last_ui_font_count.erase(Press_Last_ui_font_count.begin() + i);
            }
        }
    }
}
void Process_Event_Mouse(vec3 _direction)
{
    Ray ray(CameraPos,CameraFront);
    string name0 = ray.CheckCollider(0);
    if (name0 != "")
    {
        int ix = stoi(name0.substr(6));
        if (Crop::Existing[ix])
        {
            for (int i = 0; i < potatos.collection.size(); i++)
            {
                if (potatos.collection[i] == Collider_Layers::Collection[0]->collection_entity[ix]->Point())
                {
                    if (potatos.stages[i] >= 5)
                    {
                        potatos.collection.erase(potatos.collection.begin() + i);
                        potatos.time.erase(potatos.time.begin() + i);
                        potatos.stages.erase(potatos.stages.begin() + i);
                        Crop::Existing_Remove(ix);
                        Print::Debug("已收获相应作物:土豆");
                        Set_Plant_Score(Get_Plant_Score() + 600);
                    }
                }
            }
            for (int i = 0; i < cabbages.collection.size(); i++)
            {
                if (cabbages.collection[i] == Collider_Layers::Collection[0]->collection_entity[ix]->Point())
                {
                    if (cabbages.stages[i] >= 5)
                    {
                        cabbages.collection.erase(cabbages.collection.begin() + i);
                        cabbages.time.erase(cabbages.time.begin() + i);
                        cabbages.stages.erase(cabbages.stages.begin() + i);
                        Crop::Existing_Remove(ix);
                        Print::Debug("已收获相应作物:白菜");
                        Set_Plant_Score(Get_Plant_Score() + 500);
                    }
                }
            }
            Set.Write_Game();
        }
    }
    else
        Print::Debug("xxx");

    //std::thread thread_object(ray.CheckCollider(0));
    //thread_object.detach();
}
void Process_UIEvent_Mouse(vec3 _click_point)
{
    //cout << Event_Mouse.size() << endl;
    for (int i = 0; i < Event_Mouse.size(); i++)
    {
        if (Event_Mouse[i].CheckEvent(_click_point) && Event_Mouse[i].Function != NULL)
        {
            //cout << "???" << endl;
            //Print::Debug("剩下的");
            Event_Mouse[i].Function();
        }
    }
}
void Process_Event_MouseRight(vec3 _direction)
{
    Ray ray(CameraPos, CameraFront);
    string name0 = ray.CheckCollider(0);
    if (name0 != "")
    {
        bool pointcheck = false;
        int ix = stoi(name0.substr(6));
        if (!Crop::Existing[ix])
        {
            void* scene_ptr = (Scene_First*)scene->Get_scene(1);
            //物品种植选择
            if (scene_ptr != NULL)
            {
                Scene_First* s = (Scene_First*)scene_ptr;
                switch (s->fonts[15].mod)
                {
                case -1:
                    Crop::Existing_Add(ix);
                    potatos.time.push_back(0);
                    potatos.stages.push_back(0);
                    potatos.collection.push_back(Collider_Layers::Collection[0]->collection_entity[ix]->Point());
                    break;
                case 0:
                    Crop::Existing_Add(ix);
                    cabbages.time.push_back(0);
                    cabbages.stages.push_back(0);
                    cabbages.collection.push_back(Collider_Layers::Collection[0]->collection_entity[ix]->Point());
                    break;
                default:
                    Crop::Existing_Add(ix);
                    potatos.time.push_back(0);
                    potatos.stages.push_back(0);
                    potatos.collection.push_back(Collider_Layers::Collection[0]->collection_entity[ix]->Point());
                    break;
                }
            }
        }
        else
        {
            Print::Debug("已有相应作物");
        }
    }
    else
        Print::Debug("xxx");
}
bool F_Release = true;
void Process_Down_F(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && T_Release == true)
    {
        if(F_Release == true)
            AspotLight();
        F_Release = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        Active_shader = false;
        F_Release = true;
    }
}
void PlayerReset()
{
    PlayerPos = vec3(0.0f, 10.0f, 2.0f);
}
bool R_Release = true;
void Process_Down_R(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (R_Release == true)
            PlayerReset();
        R_Release = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
        R_Release = true;
    }
}

void Process_Down_T(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && T_Release == true)
    {
        Cursor_Input_Value_Change(-1);
        T_Release = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && T_Release == false)
    {
        T_Release = true;
    }
}

void Cursor_Input_Value_Change(int i)
{
    if (i == -1)
    {
        if (Cursor_Input_Value == 3)
        {
            //设置鼠标在当前窗体的模式
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Cursor_Input_Value = 2;
            CameraPos = PlayerPos + vec3(0.0f, 0.0f, 5.0f);
        }
        else if (Cursor_Input_Value == 2)
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Cursor_Input_Value = 1;
            glfwSetCursorPos(Window, LastX, LastY);
        }
        else
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Cursor_Input_Value = 3;
            glfwSetCursorPos(Window, LastX, LastY);
        }
    }
    else
    {
        if (i == 2)
        {
            //设置鼠标在当前窗体的模式
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Cursor_Input_Value = 2;
            CameraPos = PlayerPos + vec3(0.0f, 0.0f, 5.0f);
        }
        else if (i == 1)
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Cursor_Input_Value = 1;
            glfwSetCursorPos(Window, LastX, LastY);
        }
        else if(i == 3)
        {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Cursor_Input_Value = 3;
            glfwSetCursorPos(Window, LastX, LastY);
        }
    }
}

float jump_cout = 0.0f;
bool SPACE_Release = true;
void Process_Down_SPACE(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (SPACE_Release == true && PlayerToEarth)
            SPACE_Release = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        //SPACE_Release = true;
    }
    if (SPACE_Release == false)
    {
        Player->RigidBody(-3.0f);
        //PlayerPos += 2 * PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        jump_cout += FixedDeltaTime;
        if (jump_cout > 0.4f)
        {
            SPACE_Release = true;
            jump_cout = 0.0f;
            Player->RigidBody(7.0f);
        }
    }
}

float jump_min_count = 0.0f;
bool jump_min = false;
void PlayerJumpMin()
{
    if(PlayerToEarth)
        jump_min = true;
}
//轻微跳跃
void PlayerJumpMinX()
{
    if (jump_min == true)
    {
        Player->RigidBody(-3.0f);
        //PlayerPos += 2 * PlayerSpeed * vec3(0.0f, 1.0f, 0.0f);
        jump_min_count += FixedDeltaTime;
        if (jump_min_count > 0.1f)
        {
            jump_min = false;
            jump_min_count = 0.0f;
            Player->RigidBody(7.0f);
        }
    }
}

void Process_Down_Number(GLFWwindow* window)
{
        void* scene_ptr = (Scene_First*)scene->Get_scene(1);
    //物品切换
    if (scene_ptr != NULL)
    {
        Scene_First* s = (Scene_First*)scene_ptr;
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 0;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 0 - 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 1;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 1 - 1;
        }
        else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 2;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 2 - 1;
        }else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 3;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 3 - 1;
        }else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 4;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 4 - 1;
        }else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            int last_i = s->items_collection[s->items_current_i];
            s->fonts[last_i].mod = -1;
            s->items_current_i = 5;
            int next_i = s->items_collection[s->items_current_i];
            s->fonts[next_i].mod = -1;
            s->fonts[15].mod = 5 - 1;
        }

    }
}

