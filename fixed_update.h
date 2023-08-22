#pragma once
static bool FixedOnLoad = false;
static float FixedUpdatePerSecond = 50.0f;
void FixedTime();
double (*GetTime)();
static thread FixedUpdateThread;
static float FixedCount = 0.0f;
static int Fixededed = 0;
void FixedUpdate()
{
	Is_FixedUpdate = true;
	thread t(FixedTime);
	t.detach();
	FixedOnLoad = true;
	while (!glfwWindowShouldClose(Window) || Is_FixedTimeUpdate)
	{
		if (WindowOnLoad && FixedOnLoad)
		{
			// 每帧时间逻辑(per-frame time logic)
			//------------------------------------
			float currentFrame = glfwGetTime();
			currentFrame = GetTime();
			FixedDeltaTime = currentFrame - FixedLastFrame;
			FixedLastFrame = currentFrame;
			FixedCount += FixedDeltaTime;
			Fixededed++;
			//进程输入
			//--------
			processInput(Window);
			scene->FixedUpdate();

			float currentFrame_last = glfwGetTime();
			currentFrame_last = GetTime();
			float ss = 1 / FixedUpdatePerSecond;
			float cha = distance(currentFrame_last,currentFrame);
			float haomiao = 0.0f;
			if (cha < ss)//
			{
				FixedOnLoad = false;
				haomiao = 1000.0f * ((ss)-cha);
				Sleep(haomiao);
			}
			if (FixedCount > 1.0f)
			{
				//count << cha << endl;
				//count << currentFrame << endl;
				//count << currentFrame_last << endl;
				//count << haomiao << endl;//
				//count << 1000.0f * FixedDeltaTime << endl;
				//count << Fixededed << endl;
				Fixededed = 0;
				FixedCount = 0.0f;
				Crops_Time::FixedUpdate();

			}
			vec3 cursor_point_screenCenter = vec3(Cursor_Point.x - (SCR_WIDTH / 2), -(Cursor_Point.y - (SCR_HEIGHT / 2)), Cursor_Point.z);
			Process_UIHover_Mouse(cursor_point_screenCenter);
			//xtime xti;
			//xti.sec = 3;
			//std::this_thread::sleep_until(&xti);
			//SuspendThread(FixedUpdateThread.native_handle());
		}
		Is_FixedUpdate = false;
	}
}

void FixedTime()
{
	Is_FixedTimeUpdate = true;
	while (!glfwWindowShouldClose(Window))
	{
		if (!FixedOnLoad)
		{
			FixedOnLoad = true;
			ResumeThread(FixedUpdateThread.native_handle());
			Sleep(1000.0f / FixedUpdatePerSecond);
		}
	}
	Is_FixedTimeUpdate = false;
}