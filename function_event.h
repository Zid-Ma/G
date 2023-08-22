#pragma once
//注册鼠标事件
void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function, void* _ui_font);
//清除鼠标事件
void ClearEvent();

void PlayerJumpMin();
void PlayerJumpMinX();

void PrintHello();
void PrintHover();
void PrintPress();
void PrintRelease();
void StartGame();
void ReturnMainMenu();
void ExitGame();
void ExitGameTrue();
void ExitGameFalse();
void OpenMainMenu();
void OpenSet();
void ConfirmSet();
void ChangeLanguage();
void ChangeResolution();
void ChangeIllumination();
void ChangeShadow();
void ChangeMultisample();
void ChangeSSAO();
void ChangeFPS();
void ChangeFPSDisplay();
void ChangeCullFace();
void ChangeBlend();
void ChangeSRGB();

void ContinuGame();
void ChangeSceneCountTo2();
void ChangeSceneCountTo0();