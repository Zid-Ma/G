#pragma once

/// <summary>
/// 鼠标事件
/// </summary>
static vector<MouseClickEvent> Event_Mouse;
//注册鼠标事件
void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function)
{
    MouseClickEvent m(_type, _state, _point, _lwh, _function);
    Event_Mouse.push_back(m);
}
//清除鼠标事件
void ClearEvent()
{
    Event_Mouse.clear();
}
void PrintHello()
{
    Print::Line("HELLO");
}
void StartGame()
{
    Print::Line("开始游戏...");
    scene->Scene_Count(1);
}
void ReturnMainMenu()
{
    Print::Line("返回主菜单...");
    scene->Scene_Count(0);
}
void ExitGame()
{
    Event_Mouse.clear();//清除鼠标事件
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("点击确认按钮退出游戏...", 1, ExitGameTrue, ExitGameFalse);
}
void ExitGameTrue()
{
    Print::Line("关闭游戏...");
    WindowClose = true;
}
void ExitGameFalse()
{
    UI_MessageBox* u = ui_MessageBox;
    delete u;
    ui_MessageBox = NULL;
}
void OpenMainMenu()
{
    Print::Line("返回主菜单...");
    scene->UI_Count(0);
}
void OpenSet()
{
    Print::Line("打开设置...");
    scene->UI_Count(1);
}
void ConfirmSet()
{
    Print::Line("应用设置...");
    Set.Confirm();
    Set.Write();
}
void ChangeLanguage()
{
    Print::Line("更改语言选项:");
    UI_Font* font = (scene->Get_fonts("ChangeLanguage"));
    int *count = &(font->mod);
    if (*count < 5 && font->textures[(*count)+1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("language", *count);
    Print::Line(to_string(*count));
}
void ChangeResolution()
{
    Print::Line("更改分辨率选项");
    UI_Font* font = (scene->Get_fonts("ChangeResolution"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("resolution", *count);
    Print::Line(to_string(*count));
}
void ChangeIllumination()
{
    Print::Line("更改光照选项");
    UI_Font* font = (scene->Get_fonts("ChangeIllumination"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("illumination", *count);
    Print::Line(to_string(*count));
}
void ChangeShadow()
{
    Print::Line("更改阴影选项");
    UI_Font* font = (scene->Get_fonts("ChangeShadow"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("shadow", *count);
    Print::Line(to_string(*count));
}
void ChangeMultisample()
{
    Print::Line("更改抗锯齿选项");
    UI_Font* font = (scene->Get_fonts("ChangeMultisample"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("multisample", *count);
    Print::Line(to_string(*count));
}
void ChangeSSAO()
{
    Print::Line("更改环境光遮蔽选项");
    UI_Font* font = (scene->Get_fonts("ChangeSSAO"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("ssao", *count);
    Print::Line(to_string(*count));
}
void ChangeFPS()
{
    Print::Line("更改帧率选项");
    UI_Font* font = (scene->Get_fonts("ChangeFPS"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("fps", *count);
    Print::Line(to_string(*count));
}
void ChangeFPSDisplay()
{
    Print::Line("更改帧率显示选项");
    UI_Font* font = (scene->Get_fonts("ChangeFPSDisplay"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("fpsdisplay", *count);
    Print::Line(to_string(*count));
}
void ChangeCullFace()
{
    Print::Line("更改面剔除选项");
    UI_Font* font = (scene->Get_fonts("ChangeCullFace"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("cullface", *count);
    Print::Line(to_string(*count));
}
void ChangeBlend()
{
    Print::Line("更改混合选项");
    UI_Font* font = (scene->Get_fonts("ChangeBlend"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("blend", *count);
    Print::Line(to_string(*count));
}
void ChangeSRGB()
{
    Print::Line("更改色调映射选项");
    UI_Font* font = (scene->Get_fonts("ChangeSRGB"));
    int* count = &(font->mod);
    if (*count < 5 && font->textures[(*count) + 1] != 0)
        (*count)++;
    else
        *count = -1;
    Set.ChangeData("srgb", *count);
    Print::Line(to_string(*count));
}


void ContinuGame()
{
    Print::Line("继续游戏");
    scene->UI_Count(0);
}