#pragma once

#pragma region Register_Event

/// <summary>
/// 鼠标点击事件
/// </summary>
static vector<MouseClickEvent> Event_Mouse;
/// <summary>
/// 鼠标悬停事件
/// </summary>
static vector<MouseClickEvent> Hover_Mouse;
/// <summary>
/// 鼠标按下事件
/// </summary>
static vector<MouseClickEvent> Press_Mouse;
/// <summary>
/// 鼠标松开事件
/// </summary>
static vector<MouseClickEvent> Release_Mouse;
//注册鼠标事件
void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function, void* _ui_font)
{
    if (_state == "click")
    {
        MouseClickEvent m(_type, _state, _point, _lwh, _function);//, (UI_Font*)_ui_font
        Event_Mouse.push_back(m);
    }
    else if (_state == "press")
    {
        MouseClickEvent m(_type, _state, _point, _lwh, _function, (UI_Font*)_ui_font);
        Press_Mouse.push_back(m);
    }
    else if (_state == "release")
    {
        MouseClickEvent m(_type, _state, _point, _lwh, _function, (UI_Font*)_ui_font);
        Release_Mouse.push_back(m);
    }
    else// if (_state== "hover")
    {
        MouseClickEvent m(_type, _state, _point, _lwh, _function, (UI_Font*)_ui_font);
        Hover_Mouse.push_back(m);
    }
}
//清除鼠标事件
void ClearEvent()
{
    Event_Mouse.clear();
    Hover_Mouse.clear();
    Press_Mouse.clear();
    Release_Mouse.clear();
}
#pragma endregion

void PrintHello()
{
    Print::Debug("HELLO");
}
void PrintHover()
{
    //Print::Line("Hover");
}
void PrintPress()
{

}
void PrintRelease()
{

}
void StartGame()
{
    Print::Debug("开始游戏...");
    scene->Scene_Count(1);
}
void ReturnMainMenu()
{
    Print::Debug("返回主菜单...");
    scene->Scene_Count(0);
}
void ExitGame()
{
    Event_Mouse.clear();//清除鼠标事件
    if(ui_MessageBox == NULL)
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("点击确认按钮退出游戏...", 1, ExitGameTrue, ExitGameFalse);
}
void ExitGameTrue()
{
    Print::Debug("关闭游戏...");
    WindowClose = true;
}
void ExitGameFalse()
{
    UI_MessageBox* u = ui_MessageBox;
    delete u;
    ui_MessageBox = NULL;
}
void ExitMessageBox()
{
    UI_MessageBox* u = ui_MessageBox;
    delete u;
    ui_MessageBox = NULL;
}
void OpenMainMenu()
{
    Print::Debug("返回主菜单...");
    scene->UI_Count(0);
}
void OpenSet()
{
    Print::Debug("打开设置...");
    Set.Read();
    scene->UI_Count(1);
}
void ConfirmSet()
{
    Print::Debug("应用设置...");
    Set.Confirm();
    Set.Write();
    if (ui_MessageBox == NULL)
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("已保存设置，部分选项将在重启游戏后生效。", 0, ExitMessageBox, NULL);
}
void ChangeLanguage()
{
    Print::Debug("更改语言选项:");
    UI_Font* font = (scene->Get_fonts("ChangeLanguage"));
    int *count = &(font->mod);
    if (*count < 5 && font->textures[(*count)+1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("language", *count);
    Print::Debug(to_string(*count));
}
void ChangeResolution()
{
    Print::Debug("更改分辨率选项");
    UI_Font* font = (scene->Get_fonts("ChangeResolution"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("resolution", *count);
    Print::Debug(to_string(*count));
}
void ChangeIllumination()
{
    Print::Debug("更改光照选项");
    UI_Font* font = (scene->Get_fonts("ChangeIllumination"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("illumination", *count);
    Print::Debug(to_string(*count));
}
void ChangeShadow()
{
    Print::Debug("更改阴影选项");
    UI_Font* font = (scene->Get_fonts("ChangeShadow"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("shadow", *count);
    Print::Debug(to_string(*count));
}
void ChangeMultisample()
{
    Print::Debug("更改抗锯齿选项");
    UI_Font* font = (scene->Get_fonts("ChangeMultisample"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("multisample", *count);
    Print::Debug(to_string(*count));
}
void ChangeSSAO()
{
    Print::Debug("更改环境光遮蔽选项");
    UI_Font* font = (scene->Get_fonts("ChangeSSAO"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("ssao", *count);
    Print::Debug(to_string(*count));
}
void ChangeFPS()
{
    Print::Debug("更改帧率选项");
    UI_Font* font = (scene->Get_fonts("ChangeFPS"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("fps", *count);
    Print::Debug(to_string(*count));
}
void ChangeFPSDisplay()
{
    Print::Debug("更改帧率显示选项");
    UI_Font* font = (scene->Get_fonts("ChangeFPSDisplay"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("fpsdisplay", *count);
    Print::Debug(to_string(*count));
}
void ChangeCullFace()
{
    Print::Debug("更改面剔除选项");
    UI_Font* font = (scene->Get_fonts("ChangeCullFace"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("cullface", *count);
    Print::Debug(to_string(*count));
}
void ChangeBlend()
{
    Print::Debug("更改混合选项");
    UI_Font* font = (scene->Get_fonts("ChangeBlend"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("blend", *count);
    Print::Debug(to_string(*count));
}
void ChangeSRGB()
{
    Print::Debug("更改色调映射选项");
    UI_Font* font = (scene->Get_fonts("ChangeSRGB"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("srgb", *count);
    Print::Debug(to_string(*count));
}


void ContinuGame()
{
    Print::Debug("继续游戏");
    scene->UI_Count(0);
}
void ChangeSceneCountTo2()
{
    //Print::Line("222");
    scene->UI_Count(2);
}
void ChangeSceneCountTo0()
{
    scene->UI_Count(0);
}