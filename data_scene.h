#pragma once
//��������
static Scene* scene;

void Scene_Loading_Change(int _next_scene_count)
{
	scene->Scene_Count(_next_scene_count);
}