#pragma once
//ѭ����ʼʱ����
void Load_loop() 
{
#pragma region ÿ��ѭ����ʼ
    //std::count << "��֤�Ƿ�ѭ���ɹ�" << std::endl;

    // ÿ֡ʱ���߼�(per-frame time logic)
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
        //Print::Debug("��ǰ֡�ʣ�" + to_string(FramesPerSecond));
        FramesCount = 0.0f;
    }
    //count << currentFrame << endl;
    //��������
    //--------
    //processInput(Window);

    //��Ⱦ���
    //-------------
    //����gl�����Ļʱ�õ�����ɫ
    glClearColor(SCR_Color.x, SCR_Color.y, SCR_Color.z, SCR_Color.a);
    //�����ɫ����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //���������ģ�߻�������
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

    //�󶨶����������
    //glBindVertexArray(VAO1);


    FirstMat4 = true;
#pragma endregion
}

//ɾ����Ĭ����ײ��
void Delete_Collection()
{
    Collider_Layer_Default.Delete_Collection();
}

//ѭ����βʱ����
void End_loop() 
{
    //glfw����������������ѯIO�¼�����������/�ͷš�����ƶ��ȣ�// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// ------------------------------------------------------------------------------------------------------------------------------------------
//������ɫ����
    glfwSwapBuffers(Window);
    //SwapBuffers(hdc);
    //�����û�г���ʲô�¼�������������룬����ƶ��ȣ�
    glfwPollEvents();
}

//ѭ����ֹʱ����
void Delete_loop() 
{
    // glfw����ֹ�������ǰ���������glfw��Դ��//glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------------------------------------------------
    glfwTerminate();
}