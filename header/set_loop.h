#pragma once
//循环开始时加载
void Load_loop() 
{
#pragma region 每次循环开始
    //std::count << "验证是否循环成功" << std::endl;

    // 每帧时间逻辑(per-frame time logic)
    //------------------------------------
    float currentFrame = glfwGetTime();
    DeltaTime = currentFrame - LastFrame;
    LastFrame = currentFrame;
    TimeCout += DeltaTime;
    FramesCount++;
    if (TimeCout > 1.0f)
    {
        TimeCout = 0.0f;
        FramesPerSecond = FramesCount;
        //Print::Debug("当前帧率：" + to_string(FramesPerSecond));
        FramesCount = 0.0f;
    }
    //count << currentFrame << endl;
    //进程输入
    //--------
    //processInput(Window);

    //渲染清除
    //-------------
    //设置gl清除屏幕时用到的颜色
    glClearColor(SCR_Color.x, SCR_Color.y, SCR_Color.z, SCR_Color.a);
    //清除颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //别忘了清除模具缓冲区！
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

    //绑定顶点数组对象
    //glBindVertexArray(VAO1);


    FirstMat4 = true;
#pragma endregion
}

//删除掉默认碰撞体
void Delete_Collection()
{
    Collider_Layer_Default.Delete_Collection();
}

//循环结尾时加载
void End_loop() 
{
    //glfw：交换缓冲区和轮询IO事件（按键按下/释放、鼠标移动等）// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// ------------------------------------------------------------------------------------------------------------------------------------------
//交换颜色缓冲
    glfwSwapBuffers(Window);
    //SwapBuffers(hdc);
    //检查有没有出发什么事件（比如键盘输入，鼠标移动等）
    glfwPollEvents();
}

//循环终止时加载
void Delete_loop() 
{
    // glfw：终止，清除以前分配的所有glfw资源。//glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------------------------------------------------
    glfwTerminate();
}