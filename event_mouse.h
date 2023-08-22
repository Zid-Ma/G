#pragma once

#pragma region Register_Event

/// <summary>
/// ������¼�
/// </summary>
static vector<MouseClickEvent> Event_Mouse;
/// <summary>
/// �����ͣ�¼�
/// </summary>
static vector<MouseClickEvent> Hover_Mouse;
/// <summary>
/// ��갴���¼�
/// </summary>
static vector<MouseClickEvent> Press_Mouse;
/// <summary>
/// ����ɿ��¼�
/// </summary>
static vector<MouseClickEvent> Release_Mouse;
//ע������¼�
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
//�������¼�
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
    Print::Debug("��ʼ��Ϸ...");
    scene->Scene_Count(1);
}
void ReturnMainMenu()
{
    Print::Debug("�������˵�...");
    scene->Scene_Count(0);
}
void ExitGame()
{
    Event_Mouse.clear();//�������¼�
    if(ui_MessageBox == NULL)
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("���ȷ�ϰ�ť�˳���Ϸ...", 1, ExitGameTrue, ExitGameFalse);
}
void ExitGameTrue()
{
    Print::Debug("�ر���Ϸ...");
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
    Print::Debug("�������˵�...");
    scene->UI_Count(0);
}
void OpenSet()
{
    Print::Debug("������...");
    Set.Read();
    scene->UI_Count(1);
}
void ConfirmSet()
{
    Print::Debug("Ӧ������...");
    Set.Confirm();
    Set.Write();
    if (ui_MessageBox == NULL)
    ui_MessageBox = new UI_MessageBox();
    ui_MessageBox->Initialization("�ѱ������ã�����ѡ���������Ϸ����Ч��", 0, ExitMessageBox, NULL);
}
void ChangeLanguage()
{
    Print::Debug("��������ѡ��:");
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
    Print::Debug("���ķֱ���ѡ��");
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
    Print::Debug("���Ĺ���ѡ��");
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
    Print::Debug("������Ӱѡ��");
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
    Print::Debug("���Ŀ����ѡ��");
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
    Print::Debug("���Ļ������ڱ�ѡ��");
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
    Print::Debug("����֡��ѡ��");
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
    Print::Debug("����֡����ʾѡ��");
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
    Print::Debug("�������޳�ѡ��");
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
    Print::Debug("���Ļ��ѡ��");
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
    Print::Debug("����ɫ��ӳ��ѡ��");
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
    Print::Debug("������Ϸ");
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