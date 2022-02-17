#pragma once

/// <summary>
/// ����¼�
/// </summary>
static vector<MouseClickEvent> Event_Mouse;
//ע������¼�
void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function)
{
    MouseClickEvent m(_type, _state, _point, _lwh, _function);
    Event_Mouse.push_back(m);
}
//�������¼�
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
    Print::Line("��ʼ��Ϸ...");
    scene->Scene_Count(1);
}
void ReturnMainMenu()
{
    Print::Line("�������˵�...");
    scene->Scene_Count(0);
}
void ExitGame()
{
    Event_Mouse.clear();//�������¼�
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("���ȷ�ϰ�ť�˳���Ϸ...", 1, ExitGameTrue, ExitGameFalse);
}
void ExitGameTrue()
{
    Print::Line("�ر���Ϸ...");
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
    Print::Line("�������˵�...");
    scene->UI_Count(0);
}
void OpenSet()
{
    Print::Line("������...");
    scene->UI_Count(1);
}
void ConfirmSet()
{
    Print::Line("Ӧ������...");
    Set.Confirm();
    Set.Write();
}
void ChangeLanguage()
{
    Print::Line("��������ѡ��:");
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
    Print::Line("���ķֱ���ѡ��");
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
    Print::Line("���Ĺ���ѡ��");
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
    Print::Line("������Ӱѡ��");
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
    Print::Line("���Ŀ����ѡ��");
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
    Print::Line("���Ļ������ڱ�ѡ��");
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
    Print::Line("����֡��ѡ��");
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
    Print::Line("����֡����ʾѡ��");
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
    Print::Line("�������޳�ѡ��");
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
    Print::Line("���Ļ��ѡ��");
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
    Print::Line("����ɫ��ӳ��ѡ��");
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
    Print::Line("������Ϸ");
    scene->UI_Count(0);
}