#pragma once
//��Ҷ���
ModelX* Player;
class Scene_First
{
private:
    bool first = true;
private:
    XYKS* xyks;
    Model *xxx;
    ModelX *player;
    Model* woodenBox;
    Model* grassBox;
    Model* collider_cube;
    vec3* fpoint;
    OBJMesh* sc;
    OBJMesh* scc;
    //OBJMesh* fish;
    OBJMesh* gengdi;
public:
    vector<UI_Font> fonts;
    int ui_count = 0;
public:
    Scene_First()
    {
        First();
    }
    ~Scene_First()
    {
        Print::Debug("���ڳ���ɾ������ģ�Ͷ���...");
        delete sc;
        delete scc;
        //delete fish;
        delete gengdi;
        delete xyks;
        delete xxx;
        delete player;
        delete woodenBox;
        delete grassBox;
        delete collider_cube;
        delete[20 * 20] fpoint;
        Delete_Collection();
        Delete();
        Print::Debug("����ģ�Ͷ�����ɾ��...");
    }
public:
    void First()
    {
        if (first)
        {
            first = false;
            sc = new OBJMesh(Path_Model + "OBJ/Scene/Scene.obj", false);;//new OBJMesh(Path_Model + "OBJ/Scene/Scene.obj", false);
            scc = new OBJMesh(Path_Model + "OBJ/Scene/Scene_Collider.obj", true);
            //fish = new OBJMesh(Path_Model + "����/fish.obj", false);//new OBJMesh(Path_Model + "����/fish.obj", false);
            gengdi = new OBJMesh(Path_Model + "OBJ/GengDi/GengDi.obj", true);
            xyks = new XYKS();
            xxx = new Model(Path_Model + "����/" + "����2.txt");
            player = new ModelX(Path_Model + "����/" + "����X.txt");
            Player = player;
            woodenBox = new Model(Path_Model + "ľ����.txt");
            grassBox = new Model(Path_Model + "�ݷ���.txt");
            collider_cube = new Model(Path_Model + "������ײ��.txt");
            fpoint = new vec3[20 * 20];
            vec3 fff = vec3(-10.0f, -0.4f, -15.0f);
            float iz = fff.z;
            int count = 0;
            for (int x = 0; x < 20; x++)
            {
                for (int z = 0; z < 20; z++)
                {
                    fpoint[count] = fff;
                    count++;
                    fff.z += 1.4f;
                }
                fff.z = iz;
                fff.x += 1.4f;
            }
            gengdi->First();
            for (int i = 0; i < 20 * 20; i++)
            {
                gengdi->AddCollider(fpoint[i], &Collider_Layers::Collection[0]->collection_entity);
                Collider_Layers::Collection[0]->collection_entity.back()->name = "gengdi" + to_string(i);
            }
            Print::Debug("������ײ�����:" + to_string(Collider_Layers::Collection[0]->collection_entity.size()));
            int i;
            //cin >> i;
            InitializationFont();
            RegisterUI();
            PlayerReset();
            Cursor_Input_Value_Change(2);
        }
    }
    void Draw()
    {
        First();
        //Print::Debug(to_string(PlayerPos.x) + to_string(PlayerPos.y));
        sc->Draw(vec3(0.0f, -0.4f, 2.0f));
        scc->Draw(vec3(0.0f, -0.4f, 2.0f));
        WoodenBox(vec3(-1.0f, -1.0f, 2.0f));
        Grass(vec3(-1.0f, 0.0f, 1.0f));
        XYKS(vec3(0.0f, 0.0f, 1.0f));
        //Grass grass(vec3(1.0f, 0.0f, 2.0f));
        XYKS xyks(vec3(1.0f, 0.0f, 1.0f));
        woodenBox->Draw(vec3(0.0f, -1.0f, 2.0f));
        woodenBox->Draw(vec3(1.0f, -1.0f, 2.0f), true);
        //woodenBox->Draw(vec3(-1.0f, -1.0f, 2.0f), true);
        collider_cube->Draw(vec3(2.0f, -25.5f, 2.0f), vec3(50));
        grassBox->Draw(vec3(3.0f, -1.0f, 2.0f), true);
        //gengdi->Draws(fpoint, 25 * 25);
        for (int i = 0; i < 20 * 20; i++)
        {
            gengdi->Draw(fpoint[i]);
        }
        Crops_Time::Draw();

        //xxx->Draw(PlayerPos + vec3(1.0f));
        player->Draw(&PlayerPos);
        //fish->Draw(PlayerPos);
        //grassBox->Draw(vec3(-2.0f, -1.0f, 2.0f), true);
        ui->Use();
        ui->Use_fontShader();
        switch (ui_count)
        {
        case 0:
            UI0();
            break;
        case 1:
            UI1();
            break;
        default:
            break;
        }

    }
    void Draw_Shadow()
    {
        First();
        sc->Draw_Shadow(vec3(0.0f, -0.4f, 2.0f));
        scc->Draw_Shadow(vec3(0.0f, -0.4f, 2.0f));
        woodenBox->Draw_Shadow(vec3(-1.0f, -1.0f, 2.0f));
        grassBox->Draw_Shadow(vec3(-1.0f, 0.0f, 1.0f));
        xyks->Draw_Shadow(vec3(0.0f, 0.0f, 1.0f));
        //grassBox->Draw_Shadow(vec3(1.0f, 0.0f, 2.0f));
        xyks->Draw_Shadow(vec3(1.0f, 0.0f, 1.0f));
        woodenBox->Draw_Shadow(vec3(0.0f, -1.0f, 2.0f));
        woodenBox->Draw_Shadow(vec3(1.0f, -1.0f, 2.0f));
        //woodenBox->Draw_Shadow(vec3(-1.0f, -1.0f, 2.0f));
        grassBox->Draw_Shadow(vec3(3.0f, -1.0f, 2.0f));
        ////grassBox->Draws(fpoint, 2500);

        //xxx->Draw(PlayerPos + vec3(1.0f));
        player->Draw_Shadow(PlayerPos);
        //fish->Draw_Shadow(PlayerPos);
        //grassBox->Draw_Shadow(vec3(-2.0f, -1.0f, 2.0f));
    }
    void Draw_SSAO()
    {
        if (!Active_ssao)
        {
            return;
        }
        First();
        sc->Draw(vec3(0.0f, -0.4f, 2.0f));
        //scc->Draw(vec3(0.0f, -0.4f, 2.0f));
        woodenBox->Draw_SSAO(vec3(-1.0f, -1.0f, 2.0f), shaderGeometryPass_SSAO);
        grassBox->Draw_SSAO(vec3(-1.0f, 0.0f, 1.0f), shaderGeometryPass_SSAO);
        xyks->Draw_SSAO(vec3(0.0f, 0.0f, 1.0f), shaderGeometryPass_SSAO);
        xyks->Draw_SSAO(vec3(1.0f, 0.0f, 1.0f), shaderGeometryPass_SSAO);
        woodenBox->Draw_SSAO(vec3(0.0f, -1.0f, 2.0f), shaderGeometryPass_SSAO);
        //grassBox->Draws(fpoint, 2500);

        //xxx->Draw(PlayerPos + vec3(1.0f));
        player->Draw_SSAO(PlayerPos);
        //fish->Draw_SSAO(PlayerPos, shaderGeometryPass_SSAO);
        //grassBox->Draw_SSAO(vec3(-2.0f, -1.0f, 2.0f), shaderGeometryPass_SSAO);
    }
    void Draw_UI()
    {
        ui->Use_shader();
        ui->DrawImage(fonts[2].texture, fonts[2].point.x, fonts[2].point.y, fonts[2].lwh.x, fonts[2].lwh.y);
        ui->Use();
        //ui->DrawTexts("XYKS!!! Hello Gamer!", -(SCR_WIDTH * 0.5f) + 50, (SCR_HEIGHT * 0.5f) - 50, 16);//SCR_WIDTH
        ui->DrawTexts(to_string(Plant_Score), -(SCR_WIDTH * 0.5f) + 390 , (SCR_HEIGHT * 0.5f) - 50, 16);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    void FixedUpdate()
    {
        First();
        sc->Draw_Collider(vec3(0.0f, -0.4f, 2.0f));
        scc->Draw_Collider(vec3(0.0f, -0.4f, 2.0f));
        woodenBox->Draw_Collider(vec3(0.0f, -1.0f, 2.0f));
        woodenBox->Draw_Collider(vec3(1.0f, -1.0f, 2.0f), true);
        //woodenBox->Draw(vec3(-1.0f, -1.0f, 2.0f), true);
        collider_cube->Draw_Collider(vec3(2.0f, -25.5f, 2.0f), vec3(50));
        grassBox->Draw_Collider(vec3(3.0f, -1.0f, 2.0f), true);
        //for (int i = 0; i < 25 * 25; i++)
        //{
        //    gengdi->Draw(fpoint[i]);
        //}

        //xxx->Draw(PlayerPos + vec3(1.0f));
        player->Draw_Collider(&PlayerPos);
        //ɾ����ײ��
        Delete_Collection();
    }
public:
    int items_length = 6;
    int items_collection[6] = { 7,8,9,10,11,12 };
    int items_current_i = 0;
private:
    int ui_length0 = 17;
    int ui_collection0[20] = { 0,2,3,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    int ui_length1 = 2;
    int ui_collection1[2] = { 1,4};
public:
    void UI0()
    {
        Draw_UI(ui_length0, ui_collection0);
    }
    void UI1()
    {
        Draw_UI(ui_length1, ui_collection1);
    }
    void Draw_UI(int _length, int* _collection)
    {
        int i = 0;
        ui->Use_fontShader();
        for (int ic = 0; ic < _length; ic++)
        {
            i = _collection[ic];
            if (fonts[i].mod >= 0)
            {
                //Print::Debug(to_string(fonts[i].textures[fonts[i].mod]));
                //Print::Debug("����ɶ" + to_string(fonts[i].mod));
                ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
            }
            else
            {
                if (fonts[i].type == 0)
                {
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }

            }
            //Print::Debug("UI����ʱ������Ļ���ĵ�λ��ƫ�ƣ�x" + to_string(-1920 / 2 + 200) + "y" + to_string(1080 / 2 - 200));
        }
    }
    //������Ⱦ��ʼ��
    void InitializationFont()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        //0��һ���������
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 270, SCR_HEIGHT_CENTER_TOP - 45, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_RIGHT
        _lwh = vec3(100, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, OpenSet, "����", "", "", "", "", "");
        fonts.push_back(_f);
        //1�ڶ����������
        _point = vec3(0, -100, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ReturnMainMenu, "������ҳ", "", "", "", "", "");
        _f.point = _point;
        _f.lwh = _lwh;
        fonts.push_back(_f);
        //2
        _f = UI_Font();
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 120, SCR_HEIGHT_CENTER_TOP - 120, 0);
        _lwh = vec3(200,200,0);
        _f.content = NULL;
        GLuint texturei = 0;
        texturei = Load_texture_image(Path_Resources + "ui/MapB.png");
        _f.texture = texturei;
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //3
        _f = UI_Font();
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 120, SCR_HEIGHT_CENTER_TOP - 120, 0);
        _lwh = vec3(200, 200, 0);
        _f.name = "MiniMap";
        _f.content = NULL;
        texturei = 0;
        texturei = Bloom::cbo[0];
        _f.texture = texturei;
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //4
        _f = UI_Font();
        _point = vec3(0,100,0);
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("",_point,_lwh,32,-1, ContinuGame,"������Ϸ","","","","","");
        _f.point = _point;
        _f.lwh = _lwh;
        fonts.push_back(_f);
        //5
        _f = UI_Font();
        _point = vec3(-320, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
        _lwh = vec3(80, 80, 0);
        _f.name = "A";
        _f.content = NULL;
        texturei = 0;
        texturei = Load_texture_image(Path_Resources + "ui/��ͷ.png");
        _f.texture = texturei;
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //6
        _f = UI_Font();
        _point = vec3(-320, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
        _lwh = vec3(80, 80, 0);
        _f.name = "��Ʒ�߿�0";
        _f.content = NULL;
        texturei = 0;
        texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
        _f.texture = texturei;
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //7
        {
            _f = UI_Font();
            _point = vec3(-200, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�1";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //8
        {
            _f = UI_Font();
            _point = vec3(-100, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�2";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = 1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //9
        {
            _f = UI_Font();
            _point = vec3(0, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�3";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = 1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //10
        {
            _f = UI_Font();
            _point = vec3(100, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�4";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = 1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //11
        {
            _f = UI_Font();
            _point = vec3(200, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�5";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = 1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //12
        {
            _f = UI_Font();
            _point = vec3(300, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "��Ʒ�߿�6";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = 1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //13
        {
            _f = UI_Font();
            _point = vec3(300, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "A";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/����.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //14
        {
            _f = UI_Font();
            _point = vec3(0, 0, 0);
            _lwh = vec3(30, 30, 0);
            _f.name = "׼��";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/׼��.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //15
        {
            _f = UI_Font();
            _point = vec3(0, SCR_HEIGHT_CENTER_BOTTOM + 170, 0);
            _lwh = vec3(80, 80, 0);
            _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "����", "�ײ�", "�д�", "����", "�²�", "����");
            _f.textures[5] = Load_texture_image(Font::Load_FontToPNG("m", Path_Font + Font, "�ײ�", 32, _lwh.x, _lwh.y, &_f.length), _f.length, _lwh.x, _lwh.y, GL_RGBA);
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 0;
            fonts.push_back(_f);
        }
        //16
        {
            _f = UI_Font();
            _point = vec3(-200, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "����";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/����.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //17        
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 370, SCR_HEIGHT_CENTER_TOP - 50, 0);
            _lwh = vec3(120, 100, 0);
            _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "��ֲ�÷�:", "", "", "", "", "");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 0;
            fonts.push_back(_f);
        }
        //18
        {
            _f = UI_Font();
            _point = vec3(-100, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(80, 80, 0);
            _f.name = "�ײ�";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/�ײ�.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/��Ʒ�߿�-ѡ��.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        cout << _f.length << endl;
        for (int i = 0; i < _f.length; i++)
        {
            if (_f.content != NULL)
                cout << CharToByte(_f.content[i]);
            else
                break;
            //cout << to_string(f[i]) + "<:>" ;
        }
        //Load_texture(&ti, Path_Font + "STB.png");
    }
public:
    void UI_Count(int _count)
    {
        ui_count = _count;
        ClearEvent();
        RegisterUI();
    }

    void RegisterUI()
    {
        for (int i = 0; i < fonts.size(); i++)
        {
            if (fonts[i].function != NULL)
                RegisterEvent("", "press", fonts[i].point, fonts[i].lwh, fonts[i].function);
        }
    }
private:
    void Delete()
    {
        for (int i = 0; i < fonts.size(); i++)
        {
            delete[] fonts[i].content;
        }
    }
};

class Scene_Star
{
private:
    bool first = true;
private:
    bool last_messagebox = false;
public:
    int ui_count = 0;
    vector<UI_Font> fonts;
public:
    Scene_Star()
    {
        First();
    }

    ~Scene_Star()
    {
        Delete();
    }

    void First()
    {
        if (first)
        {
            InitializationFont();
            RegisterUI();
            Cursor_Input_Value_Change(1);
            first = false;
        }
    }

    void Draw()
    {
        ui->Use();
        ui->Use_fontShader();
        switch (ui_count)
        {
        case 0:
            UI0();
            break;
        case 1:
            UI1();
            break;
        default:
            break;
        }
        if (ui_MessageBox != NULL)
        {
            glDisable(GL_DEPTH_TEST);
            ui_MessageBox->DrawMessageBox();
            last_messagebox = true;
            glEnable(GL_DEPTH_TEST);
        }
        else if(last_messagebox)
        {
            UI_Count(0);
            last_messagebox = false;
        }
    }

    void Draw_Shadow()
    {

    }

    void Draw_SSAO()
    {

    }

    void Draw_UI()
    {

    }
    void FixedUpdate()
    {

    }
private:
    int ui_length0 = 3;
    int ui_collection0[3] = { 0,1,2 };
    int ui_length1 = 24;
    int ui_collection1[24] = { 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 };
private:
    void UI0()
    {
        Draw_UI(ui_length0, ui_collection0);
    }

    void UI1()
    {
        Draw_UI(ui_length1, ui_collection1);
    }

    void Draw_UI(int _length, int* _collection)
    {
        int i = 0;
        for (int ic = 0; ic < _length; ic++)
        {
            i = _collection[ic];
            if (fonts[i].mod >= 0)
            {
                //Print::Debug(to_string(fonts[i].textures[fonts[i].mod]));
                //Print::Debug("����ɶ" + to_string(fonts[i].mod));
                ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
            }
            else
            {
                ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
            }
            //Print::Debug("UI����ʱ������Ļ���ĵ�λ��ƫ�ƣ�x" + to_string(-1920 / 2 + 200) + "y" + to_string(1080 / 2 - 200));
        }
    }
private:
    //������Ⱦ��ʼ��
    void InitializationFont()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        unsigned char* content;
        //��0���������
        _point = vec3(0, 100, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1,StartGame,"��ʼ��Ϸ","","","","","");
        fonts.push_back(_f);
        //1
        _point = vec3(0, 00, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(100, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, OpenSet, "����", "", "", "", "", "");
        fonts.push_back(_f);
        //2
        _point = vec3(0, -100, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ExitGame, "�˳���Ϸ", "", "", "", "", "");
        fonts.push_back(_f);
        //3
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 100, SCR_HEIGHT_CENTER_TOP - 50, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, OpenMainMenu, "����", "", "", "", "", "");
        fonts.push_back(_f);
        int pianyi = 50;
        //4
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 2), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "����", "", "", "", "", "");
        fonts.push_back(_f);
        //5
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 2), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeLanguage", _point, _lwh, 32, -1, ChangeLanguage, "����", "English", "", "", "", "");
        fonts.push_back(_f);
        //6
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 3), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "�ֱ���", "", "", "", "", "");
        fonts.push_back(_f);
        //7
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 3), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeResolution", _point, _lwh, 32, -1, ChangeResolution, "1920*1080", "1024*768", "2556*1440", "", "", "");
        fonts.push_back(_f);
        //8
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 4), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "����", "", "", "", "", "");
        fonts.push_back(_f);
        //9
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 4), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeIllumination", _point, _lwh, 32, -1, ChangeIllumination, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //10
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 5), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "��Ӱ", "", "", "", "", "");
        fonts.push_back(_f);
        //11
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 5), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeShadow", _point, _lwh, 32, -1, ChangeShadow, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //12
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 6), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "�����", "", "", "", "", "");
        fonts.push_back(_f);
        //13
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 6), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeMultisample", _point, _lwh, 32, -1, ChangeMultisample, "��", "�ر�", "2X", "4X", "8X", "16X");
        fonts.push_back(_f);
        //14
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 225, SCR_HEIGHT_CENTER_TOP - (pianyi * 7), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(200, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "�������ڱ�", "", "", "", "", "");
        fonts.push_back(_f);
        //15
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 7), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeSSAO", _point, _lwh, 32, -1, ChangeSSAO, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //16
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 8), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "֡��", "", "", "", "", "");
        fonts.push_back(_f);
        //17
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 8), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeFPS", _point, _lwh, 32, -1, ChangeFPS, "������Ļ", "60", "144", "240", "��������", "");
        fonts.push_back(_f);
        //18
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 9), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "֡����ʾ", "", "", "", "", "");
        fonts.push_back(_f);
        //19
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 9), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeFPSDisplay", _point, _lwh, 32, -1, ChangeFPSDisplay, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //20
        _point = vec3(SCR_WIDTH_CENTER_RIGHT - 100, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ConfirmSet, "Ӧ��", "", "", "", "", "");
        fonts.push_back(_f);
        //21
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 10), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "���", "", "", "", "", "");
        fonts.push_back(_f);
        //22
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 10), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeBlend", _point, _lwh, 32, -1, ChangeBlend, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //23
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 11), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "���޳�", "", "", "", "", "");
        fonts.push_back(_f);
        //24
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 11), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeCullFace", _point, _lwh, 32, -1, ChangeCullFace, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //25
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 12), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "ɫ��ӳ��", "", "", "", "", "");
        fonts.push_back(_f);
        //26
        _point = vec3(0, SCR_HEIGHT_CENTER_TOP - (pianyi * 12), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeSRGB", _point, _lwh, 32, -1, ChangeSRGB, "��", "�ر�", "", "", "", "");
        fonts.push_back(_f);
        //cout << _f.length << endl;
        //for (int i = 0; i < _f.length; i++)
        //{
        //    cout << CharToByte(_f.content[i]);
        //    //cout << to_string(f[i]) + "<:>" ;
        //}
        //Load_texture(&ti, Path_Font + "STB.png");
    }
public:
    void UI_Count(int _count)
    {
        ui_count = _count;
        ClearEvent();
        RegisterUI();
    }

    void RegisterUI()
    {
        int _length = 0;
        int* _collection = NULL;
        switch (ui_count)
        {
        case 0:
            _length = ui_length0;
            _collection = ui_collection0;
            break;
        case 1:
            _length = ui_length1;
            _collection = ui_collection1;
            break;
        }
        for (int i = 0; i < _length; i++)
        {
            if (fonts[_collection[i]].function != NULL)
                RegisterEvent("", "press", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].function);
        }
    }
  private:
      void Delete()
      {
          Print::Line("���ڳ���ɾ����������...");
          try {
              for (int i = 0; i < fonts.size(); i++)
              {
                  delete[] fonts[i].content;
              }
              Print::Line("ɾ���������ݳɹ�.");
          }
          catch (exception e)
          {
              Print::Line("ɾ����������ʱ����...(���ڿ�����ģʽʱ����ʾ������Ϣ)");
              Print::Exception(e.what());
          }
      }
};

class Scene
{
private:
    bool first = true;
    bool onload = false;
    int scene_count = 0;
    int last_scene_count = 0;
private:
    vector<void*> scenes;
private:
    typedef Scene_Star s0;
    typedef Scene_First s1;
public:
    Scene()
    {
        First();
    }

    ~Scene()
    {
        Delete();
    }

    void First()
    {
        if (first)
        {
            for (int i = 0; i < 10; i++)
            {
                scenes.push_back(NULL);
            }
            scene_count = 0;
            last_scene_count = 0;
            New_Scene(0);
            first = false;
            onload = true;
        }
    }
public:
    void Draw()
    {
        switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw();
            break;
        case 1:
            ((s1*)scenes[1])->Draw();
            break;
        default:
            break;
        }
    }

    void Draw_Shadow()
    {
        switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_Shadow();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_Shadow();
            break;
        default:
            break;
        }
    }

    void Draw_SSAO()
    {
        switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_SSAO();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_SSAO();
            break;
        default:
            break;
        }
    }

    void Draw_UI()
    {
        switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_UI();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_UI();
            break;
        default:
            break;
        }
    }
    void FixedUpdate()
    {
        if (onload)
        {
            switch (scene_count)
            {
            case 0:
                ((s0*)scenes[0])->FixedUpdate();
                break;
            case 1:
                ((s1*)scenes[1])->FixedUpdate();
                break;
            default:
                break;
            }
        }
    }
private:
    void New_Scene(int _count)
    {
        switch (_count)
        {
        case 0:
            scenes[_count] = new s0();
            break;
        case 1:
            scenes[_count] = new s1();
            break;
        default:
            break;
        }
    }

    void Delete_Scene(int _count)
    {
        switch (_count)
        {
        case 0:
            delete (s0*)scenes[_count];
            break;
        case 1:
            delete (s1*)scenes[_count];
            break;
        }
        scenes[_count] = NULL;
    }

    void Delete()
    {
        for (int i = 0; i < scenes.size(); i++)
        {
            if (scenes[i] != NULL)
                Delete_Scene(i);
        }
    }
public:
    void Scene_Count(int _count)
    {
        if (_count != scene_count)
        {
            onload = false;
            ClearEvent();
            last_scene_count = scene_count;
            scene_count = _count;
            New_Scene(_count);
            Delete_Scene(last_scene_count);
            onload = true;
        }
    }
    void UI_Count(int _count)
    {
        switch (scene_count)
        {
        case 0:
            ((s0*)scenes[scene_count])->UI_Count(_count);
            break;
        case 1:
            ((s1*)scenes[scene_count])->UI_Count(_count);
            break;
        }
        set_path();
    }
    void* Get_scene()
    {
        return scenes[scene_count];
    }
    void* Get_scene(int _scene_count)
    {
        if (_scene_count == scene_count)
            return scenes[scene_count];
        else
            return NULL;
    }
    UI_Font* Get_fonts(string _name)
    {
        vector<UI_Font>* fonts;
        switch (scene_count)
        {
        case 0:
            fonts = &(((s0*)scenes[scene_count])->fonts);
            break;
        case 1:
            fonts = &(((s1*)scenes[scene_count])->fonts);
            break;
        }
        //Print::Debug("daxiao:" + to_string(fonts->size()));
        for (int i = 0; i < fonts->size(); i++)
        {
            //Print::Debug((*fonts)[i].name);
            if ((*fonts)[i].name == _name)
            {
                Print::Debug(to_string(i));
                return &((*fonts)[i]);
            }
        }
        Print::Debug("δ�ҵ���Ӧ��fonts UI����");
    }
};