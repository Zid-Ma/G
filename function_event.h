#pragma once
//ע������¼�
void RegisterEvent(string _type, string _state, vec3 _point, vec3 _lwh, void* _function);
//�������¼�
void ClearEvent();

void PlayerJumpMin();
void PlayerJumpMinX();

void PrintHello();
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