#pragma once
//玩家对象
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
        Print::Debug("正在尝试删除场景模型对象...");
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
        Print::Debug("场景模型对象已删除...");
    }
public:
    void First()
    {
        if (first)
        {
            first = false;
            sc = new OBJMesh(Path_Model + "OBJ/Scene/Scene.obj", false);;//new OBJMesh(Path_Model + "OBJ/Scene/Scene.obj", false);
            scc = new OBJMesh(Path_Model + "OBJ/Scene/Scene_Collider.obj", true);
            //fish = new OBJMesh(Path_Model + "主角/fish.obj", false);//new OBJMesh(Path_Model + "主角/fish.obj", false);
            gengdi = new OBJMesh(Path_Model + "OBJ/GengDi/GengDi.obj", true);
            xyks = new XYKS();
            xxx = new Model(Path_Model + "主角/" + "主角2.txt");
            player = new ModelX(Path_Model + "主角/" + "主角X.txt");
            Player = player;
            woodenBox = new Model(Path_Model + "木箱子.txt");
            grassBox = new Model(Path_Model + "草方块.txt");
            collider_cube = new Model(Path_Model + "方形碰撞体.txt");
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
                bool _gengdiCollider = gengdi->AddCollider(fpoint[i], &Collider_Layers::Collection[0]->collection_entity);
                if(_gengdiCollider)
                Collider_Layers::Collection[0]->collection_entity.back()->name = "gengdi" + to_string(i);
            }
            Print::Debug("耕地碰撞体个数:" + to_string(Collider_Layers::Collection[0]->collection_entity.size()));
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
        case 2:
            UI2();
            break;
        default:
            break;
        }
        if (fonts[3].name == "MiniMap")//(fonts[i].name == "MiniMap" && false)
        {
            ui->Use_shader();
            MiniMapChange(vec2(PlayerPos.x, PlayerPos.z), 0.1f);
            ui->DrawImage_MiniMap(fonts[3].texture, fonts[3].point.x, fonts[3].point.y, fonts[3].lwh.x, fonts[3].lwh.y, MiniMap_Point1, MiniMap_Point2, MiniMap_Point3, MiniMap_Point4);
            ui->Use();
        }
        ui->Use_shader();
        ui->DrawImage(fonts[2].texture, fonts[2].point.x, fonts[2].point.y, fonts[2].lwh.x, fonts[2].lwh.y);
        ui->Use();
        //ui->DrawTexts("XYKS!!! Hello Gamer!", -(SCR_WIDTH * 0.5f) + 50, (SCR_HEIGHT * 0.5f) - 50, 16);//SCR_WIDTH
        ui->DrawTexts(to_string(Get_Plant_Score()), -(SCR_WIDTH * 0.5f) + 112 , (SCR_HEIGHT * 0.5f) - 209 - 15, 14);
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
        //删除碰撞体
        Delete_Collection();
    }
public:
    int items_length = 6;
    int items_collection[6] = { 7,8,9,10,11,12 };
    int items_current_i = 0;
private:
    int ui_length0 = 21;
    int ui_collection0[30] = { 0,2,2,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20,22,23,24};
    int ui_length1 = 2;
    int ui_collection1[2] = { 1,4};
    int ui_length2 = 2;
    int ui_collection2[3] = {19,21};
public:
    void UI0()
    {
        Draw_UI(ui_length0, ui_collection0);
    }
    void UI1()
    {
        Draw_UI(ui_length1, ui_collection1);
    }
    void UI2()
    {
        //Print::Line("渲染UI2");
        Draw_UI(ui_length2,ui_collection2);
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
                if (fonts[i].type == 0)
                {
                    //Print::Debug(to_string(fonts[i].textures[fonts[i].mod]));
                    //Print::Debug("还有啥" + to_string(fonts[i].mod));
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            else
            {
                //当type为0时，渲染字体
                if (fonts[i].type == 0)
                {
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            //Print::Debug("UI创建时基于屏幕中心的位置偏移：x" + to_string(-1920 / 2 + 200) + "y" + to_string(1080 / 2 - 200));
        }
    }
    //字体渲染初始化
    void InitializationFont()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        //0第一个字体加载
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 367, SCR_HEIGHT_CENTER_TOP - 44, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_RIGHT
        _lwh = vec3(40, 30, 0);
        _f = New_UI_Font("", _point, _lwh, 14, -1, OpenSet, "设置", "", "", "", "", "");
        fonts.push_back(_f);
        //1第二个字体加载
        _point = vec3(0, -100, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ReturnMainMenu, "返回主页", "", "", "", "", "");
        _f.point = _point;
        _f.lwh = _lwh;
        fonts.push_back(_f);
        //2
        _f = UI_Font();
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 112, SCR_HEIGHT_CENTER_TOP - 104, 0);
        _lwh = vec3(170,170,0);
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
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 112, SCR_HEIGHT_CENTER_TOP - 104, 0);
        _lwh = vec3(160, 160, 0);
        _f.name = "MiniMap";
        _f.content = NULL;
        texturei = 0;
        //texturei = Bloom::cbo[0];
        texturei = Load_texture_image(Path_Resources + "ui/Map.png");
        Print::Debug("值" + to_string(texturei));
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
        _f = New_UI_Font("",_point,_lwh,32,-1, ContinuGame,"继续游戏","","","","","");
        _f.point = _point;
        _f.lwh = _lwh;
        fonts.push_back(_f);
        //5
        _f = UI_Font();
        _point = vec3(-150 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
        _lwh = vec3(50, 50, 0);
        _f.name = "A";
        _f.content = NULL;
        texturei = 0;
        texturei = Load_texture_image(Path_Resources + "ui/锄头.png");
        _f.texture = texturei;
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //6
        _f = UI_Font();
        _point = vec3(-150 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
        _lwh = vec3(50, 50, 0);
        _f.name = "物品边框0";
        _f.content = NULL;
        texturei = 0;
        texturei = Load_texture_image(Path_Resources + "ui/物品边框.png");
        _f.texture = texturei;
        _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
        _f.point = _point;
        _f.lwh = _lwh;
        _f.function = NULL;
        _f.mod = -1;
        _f.type = 1;
        fonts.push_back(_f);
        //7
        {
            _f = UI_Font();
            _point = vec3(-90 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框1";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(-30 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框2";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(30 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框3";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(90 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框4";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(150 - 60, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框5";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(150, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "物品边框6";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框.png");
            _f.textures[1] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(150, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "A";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/镰刀.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _f.name = "准星";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/准星.png");
            _f.texture = texturei;
            //_f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
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
            _point = vec3(0, SCR_HEIGHT_CENTER_BOTTOM + 130, 0);
            _lwh = vec3(30, 30, 0);
            _f = New_UI_Font("", _point, _lwh, 14, -1, NULL, "土豆", "白菜", "葱葱", "蒜苗", "韭菜", "镰刀");
            _f.textures[5] = Load_texture_image(Font::Load_FontToPNG("m", Path_Font + Font, "白菜", 14, _lwh.x, _lwh.y, &_f.length), _f.length, _lwh.x, _lwh.y, GL_RGBA);
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
            _point = vec3(-150, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "土豆";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/土豆.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/NULL.png");
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
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 112, SCR_HEIGHT_CENTER_TOP - 209, 0);
            _lwh = vec3(70, 30, 0);
            _f = New_UI_Font("", _point, _lwh, 14, -1, NULL, "种植得分:", "", "", "", "", "");
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
            _point = vec3(-90, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);
            _lwh = vec3(50, 50, 0);
            _f.name = "白菜";
            _f.content = NULL;
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/白菜.png");
            _f.texture = texturei;
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/物品边框-选中.png");
            _f.textures[0] = Load_texture_image(Path_Resources + "ui/NULL.png");
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = NULL;
            _f.mod = -1;
            _f.type = 1;
            fonts.push_back(_f);
        }
        //19
        {
            _f = UI_Font();
            _point = vec3(0, 0, 0);
            _lwh = vec3(800, 200, 0);
            _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "我大概知道是怎么回事，或又不知道是怎么回事。涌上心头的烦意大概远比一些言情小说里面的故事更让我难过。", "", "", "", "", "");//，或又不知道是怎么回事。涌上心头的烦意大概远比一些言情小说里面的故事更让我难过。
            fonts.push_back(_f);
        }
        //20
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 297, SCR_HEIGHT_CENTER_TOP - 44, 0);
            _lwh = vec3(70, 30, 0);
            _f = New_UI_Font("", _point, _lwh, 14, -1, ChangeSceneCountTo2, "背景故事", "", "", "", "", "");
            _f.function = ChangeSceneCountTo2;
            fonts.push_back(_f);
        }
        //21
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 400, SCR_HEIGHT_CENTER_TOP - 80, 0);
            _lwh = vec3(100, 40, 0);
            _f = New_UI_Font("", _point, _lwh, 32, -1, ChangeSceneCountTo0, "返回", "", "", "", "", "");
            _f.function = ChangeSceneCountTo0;
            fonts.push_back(_f);
        }
        //22
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 237, SCR_HEIGHT_CENTER_TOP - 44, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_RIGHT
            _lwh = vec3(50, 50, 0);
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = OpenSet;
            _f.mod = -1;
            _f.type = 1;
            _f.name = "背包背景图片";
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/背包.png");
            _f.texture = texturei;
            fonts.push_back(_f);
        }
        //23
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 297, SCR_HEIGHT_CENTER_TOP - 44, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_RIGHT
            _lwh = vec3(50, 50, 0);
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = OpenSet;
            _f.mod = -1;
            _f.type = 1;
            _f.name = "信息背景图片";
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/小纸张.png");
            _f.texture = texturei;
            fonts.push_back(_f);
        }
        //24
        {
            _f = UI_Font();
            _point = vec3(SCR_WIDTH_CENTER_LEFT + 367, SCR_HEIGHT_CENTER_TOP - 44, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_RIGHT
            _lwh = vec3(50, 50, 0);
            _f.point = _point;
            _f.lwh = _lwh;
            _f.function = OpenSet;
            _f.mod = -1;
            _f.type = 1;
            _f.name = "设置背景图片";
            texturei = 0;
            texturei = Load_texture_image(Path_Resources + "ui/齿轮.png");
            _f.texture = texturei;
            fonts.push_back(_f);
        }
        Print::Debug(to_string(_f.length));
        for (int i = 0; i < _f.length; i++)
        {
            if (_f.content != NULL)
                cout << CharToByte(_f.content[i]);
            else
                break;
            //count << to_string(f[i]) + "<:>" ;
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
        case 2:
            _length = ui_length2;
            _collection = ui_collection2;
            break;
        }
        for (int i = 0; i < _length; i++)
        {
            if (fonts[_collection[i]].function != NULL)
                RegisterEvent("", "click", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].function, (void*)&(fonts[_collection[i]]));
            if(fonts[_collection[i]].hover != NULL)
                RegisterEvent("", "hover", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].hover, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].press != NULL)
                RegisterEvent("", "press", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].press, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].release != NULL)
                RegisterEvent("", "release", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].release, (void*)&(fonts[_collection[i]]));
        }
        //for (int i = 0; i < fonts.size(); i++)
        //{
        //    if (fonts[i].function != NULL)
        //        RegisterEvent("", "press", fonts[i].point, fonts[i].lwh, fonts[i].function);
        //}
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
    ModelX* start_movie;
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
            start_movie = new ModelX(Path_Model + "movie/start/" + "startX.txt");
            InitializationFont();
            RegisterUI();
            Cursor_Input_Value_Change(1);
            first = false;
        }
    }

    void Draw()
    {
        start_movie->Draw(vec3(0.0f, -1.0f, 0.0f), vec3(5.1f, 3.0f, 1.0f));
    }

    void Draw_Shadow()
    {

    }

    void Draw_SSAO()
    {

    }

    void Draw_UI()
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
        else if (last_messagebox)
        {
            UI_Count(0);
            last_messagebox = false;
        }
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
                if (fonts[i].type == 0)
                {
                    //Print::Debug(to_string(fonts[i].textures[fonts[i].mod]));
                    //Print::Debug("还有啥" + to_string(fonts[i].mod));
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            else
            {
                if (fonts[i].type == 0)
                {
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            //Print::Debug("UI创建时基于屏幕中心的位置偏移：x" + to_string(-1920 / 2 + 200) + "y" + to_string(1080 / 2 - 200));
        }
    }
private:
    //字体渲染初始化
    void InitializationFont()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        unsigned char* content;
        //设置中间的字体偏移
        int zhongjianpianyi = 150;
        //第0个字体加载
        _point = vec3(0, 100, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1,StartGame,"开始游戏","","","","","");
        fonts.push_back(_f);
        //1
        _point = vec3(0, 00, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(100, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, OpenSet, "设置", "", "", "", "", "");
        fonts.push_back(_f);
        //2
        _point = vec3(0, -100, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ExitGame, "退出游戏", "", "", "", "", "");
        fonts.push_back(_f);
        //3
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 100, SCR_HEIGHT_CENTER_TOP - 50, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, OpenMainMenu, "返回", "", "", "", "", "");
        fonts.push_back(_f);
        int pianyi = 50;
        //4
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 2), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "语言", "", "", "", "", "");
        fonts.push_back(_f);
        //5
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 2), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeLanguage", _point, _lwh, 32, -1, ChangeLanguage, "中文", "English", "", "", "", "");
        fonts.push_back(_f);
        //6
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 3), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "分辨率", "", "", "", "", "");
        fonts.push_back(_f);
        //7
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 3), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeResolution", _point, _lwh, 32, -1, ChangeResolution, "1920*1080", "1024*768", "2556*1440", "", "", "");
        fonts.push_back(_f);
        //8
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 4), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "光照", "", "", "", "", "");
        fonts.push_back(_f);
        //9
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 4), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeIllumination", _point, _lwh, 32, -1, ChangeIllumination, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //10
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 5), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "阴影", "", "", "", "", "");
        fonts.push_back(_f);
        //11
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 5), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeShadow", _point, _lwh, 32, -1, ChangeShadow, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //12
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 6), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "抗锯齿", "", "", "", "", "");
        fonts.push_back(_f);
        //13
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 6), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeMultisample", _point, _lwh, 32, -1, ChangeMultisample, "打开", "关闭", "2X", "4X", "8X", "16X");
        fonts.push_back(_f);
        //14
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 225, SCR_HEIGHT_CENTER_TOP - (pianyi * 7), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(200, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "环境光遮蔽", "", "", "", "", "");
        fonts.push_back(_f);
        //15
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 7), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeSSAO", _point, _lwh, 32, -1, ChangeSSAO, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //16
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 8), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "帧率", "", "", "", "", "");
        fonts.push_back(_f);
        //17
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 8), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeFPS", _point, _lwh, 32, -1, ChangeFPS, "跟随屏幕", "30", "60", "144", "240", "不做限制");
        fonts.push_back(_f);
        //18
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 9), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "帧率显示", "", "", "", "", "");
        fonts.push_back(_f);
        //19
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 9), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeFPSDisplay", _point, _lwh, 32, -1, ChangeFPSDisplay, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //20
        _point = vec3(SCR_WIDTH_CENTER_RIGHT - 100, SCR_HEIGHT_CENTER_BOTTOM + 50, 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, ConfirmSet, "应用", "", "", "", "", "");
        fonts.push_back(_f);
        //21
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 10), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "混合", "", "", "", "", "");
        fonts.push_back(_f);
        //22
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 10), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeBlend", _point, _lwh, 32, -1, ChangeBlend, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //23
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 11), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "面剔除", "", "", "", "", "");
        fonts.push_back(_f);
        //24
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 11), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeCullFace", _point, _lwh, 32, -1, ChangeCullFace, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //25
        _point = vec3(SCR_WIDTH_CENTER_LEFT + 200, SCR_HEIGHT_CENTER_TOP - (pianyi * 12), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, NULL, "色调映射", "", "", "", "", "");
        fonts.push_back(_f);
        //26
        _point = vec3(zhongjianpianyi, SCR_HEIGHT_CENTER_TOP - (pianyi * 12), 0);//SCR_WIDTH_CENTER_LEFT    //SCR_HEIGHT_CENTER_TOP
        _lwh = vec3(150, 50, 0);
        _f = New_UI_Font("ChangeSRGB", _point, _lwh, 32, -1, ChangeSRGB, "打开", "关闭", "", "", "", "");
        fonts.push_back(_f);
        //count << _f.length << endl;
        //for (int i = 0; i < _f.length; i++)
        //{
        //    count << CharToByte(_f.content[i]);
        //    //count << to_string(f[i]) + "<:>" ;
        //}
        //Load_texture(&ti, Path_Font + "STB.png");
    }
public:
    void UI_Count(int _count)
    {
        ui_count = _count;
        ClearEvent();
        //当打开设置界面时，更改参数为现有参数
        if (_count == 1)
        {
            int _collection[11] = {5,7,9,11,13,15,17,19,22,24,26};
            int i = 0;
            fonts[_collection[i]].mod = atoi(Language.c_str()); i++;
            switch (SCR_WIDTH)
            {
            case 1920:
                fonts[_collection[i]].mod = -1; i++;
                break;
            case 1024:
                fonts[_collection[i]].mod = 0; i++;
                break;
            case 2556:
                fonts[_collection[i]].mod = 1; i++;
                break;
            case 0:
                fonts[_collection[i]].mod = 2; i++;
                break;
            case 1:
                fonts[_collection[i]].mod = 3; i++;
                break;
            default:
                fonts[_collection[i]].mod = 4; i++;
                break;
            }
            fonts[_collection[i]].mod = BoolToInt(ActiveLight); i++;
            fonts[_collection[i]].mod = BoolToInt(Active_Shadow); i++;
            switch (Multisampling)
            {
            case 16:
                fonts[_collection[i]].mod = -1; i++;
                break;
            case 0:
                fonts[_collection[i]].mod = 0; i++;
                break;
            case 2:
                fonts[_collection[i]].mod = 1; i++;
                break;
            case 4:
                fonts[_collection[i]].mod = 2; i++;
                break;
            case 8:
                fonts[_collection[i]].mod = 3; i++;
                break;
            case 32:
                fonts[_collection[i]].mod = 4; i++;
                break;
            default:
                fonts[_collection[i]].mod = -1; i++;
                break;
            }
            fonts[_collection[i]].mod = BoolToInt(Active_ssao); i++;
            switch (FramesPerSecondX)
            {
            case -1:
                fonts[_collection[i]].mod = -1; i++;
                break;
            case 30:
                fonts[_collection[i]].mod = 0; i++;
                break;
            case 60:
                fonts[_collection[i]].mod = 1; i++;
                break;
            case 144:
                fonts[_collection[i]].mod = 2; i++;
                break;
            case 240:
                fonts[_collection[i]].mod = 3; i++;
                break;
            case 0:
                fonts[_collection[i]].mod = 4; i++;
                break;
            default:
                fonts[_collection[i]].mod = -1; i++;
                break;
            }
            fonts[_collection[i]].mod = BoolToInt(FramesPerSecondDisplay); i++;
            fonts[_collection[i]].mod = BoolToInt(Active_blend); i++;
            fonts[_collection[i]].mod = BoolToInt(Active_cull_face); i++;
            fonts[_collection[i]].mod = BoolToInt(Active_srgb); i++;
        }
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
                RegisterEvent("", "click", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].function, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].hover != NULL)
                RegisterEvent("", "hover", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].hover, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].press != NULL)
                RegisterEvent("", "press", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].press, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].release != NULL)
                RegisterEvent("", "release", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].release, (void*)&(fonts[_collection[i]]));
        }
    }
  private:
      void Delete()
      {
          Print::Debug("正在尝试删除场景数据...");
          try {
              for (int i = 0; i < fonts.size(); i++)
              {
                  delete[] fonts[i].content;
              }
              Print::Debug("删除场景数据成功.");
          }
          catch (exception e)
          {
              Print::Debug("删除场景数据时出错...(处于开发者模式时将显示错误信息)");
              Print::Exception(e.what());
          }
      }
};


void Scene_Loading_Change(int _next_scene_count);
class Scene_Loading
{
private:
    bool first = true;
private:
    bool last_messagebox = false;
public:
    int next_scene = 0;
    int next_scene_count = 50;
    int ui_count = 0;
    vector<UI_Font> fonts;
public:
    Scene_Loading()
    {
        First();
    }

    ~Scene_Loading()
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

    void NextScene(int _count)
    {
        next_scene = _count;
    }

    void Draw()
    {
        //Print::Line("loading...");
        fixed_change_scene_count++;
        if (fixed_change_scene_count > next_scene_count)
        {
            Scene_Loading_Change(next_scene);
            fixed_change_scene_count = 0;
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
        else if (last_messagebox)
        {
            UI_Count(0);
            last_messagebox = false;
        }
    }
    int fixed_change_scene_count = 0;
    void FixedUpdate()
    {

    }
private:
    int ui_length0 = 1;
    int ui_collection0[1] = { 0};
    int ui_length1 = 0;
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
                if (fonts[i].type == 0)
                {
                    //Print::Debug(to_string(fonts[i].textures[fonts[i].mod]));
                    //Print::Debug("还有啥" + to_string(fonts[i].mod));
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].textures[fonts[i].mod], fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            else
            {
                if (fonts[i].type == 0)
                {
                    ui->FontColor(fonts[i].color);
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                }
                else
                {
                    ui->Use_shader();
                    ui->DrawImage(fonts[i].texture, fonts[i].point.x, fonts[i].point.y, fonts[i].lwh.x, fonts[i].lwh.y);
                    ui->Use_fontShader();
                }
            }
            //Print::Debug("UI创建时基于屏幕中心的位置偏移：x" + to_string(-1920 / 2 + 200) + "y" + to_string(1080 / 2 - 200));
        }
    }
private:
    //字体渲染初始化
    void InitializationFont()
    {
        UI_Font _f;
        vec3 _point;
        vec3 _lwh;
        unsigned char* content;
        //第0个字体加载
        _point = vec3(SCR_WIDTH_CENTER_RIGHT - 200, SCR_HEIGHT_CENTER_TOP - 50, 0);//-(SCR_WIDTH * 0.5f) //(SCR_HEIGHT * 0.5f)
        _lwh = vec3(250, 50, 0);
        _f = New_UI_Font("", _point, _lwh, 32, -1, StartGame, "加载中,请稍后...", "", "", "", "", "");
        fonts.push_back(_f);
        //count << _f.length << endl;
        //for (int i = 0; i < _f.length; i++)
        //{
        //    count << CharToByte(_f.content[i]);
        //    //count << to_string(f[i]) + "<:>" ;
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
                RegisterEvent("", "click", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].function, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].hover != NULL)
                RegisterEvent("", "hover", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].hover, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].press != NULL)
                RegisterEvent("", "press", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].press, (void*)&(fonts[_collection[i]]));
            if (fonts[_collection[i]].release != NULL)
                RegisterEvent("", "release", fonts[_collection[i]].point, fonts[_collection[i]].lwh, fonts[_collection[i]].release, (void*)&(fonts[_collection[i]]));
        }
    }
private:
    void Delete()
    {
        Print::Debug("正在尝试删除场景数据...");
        try {
            for (int i = 0; i < fonts.size(); i++)
            {
                delete[] fonts[i].content;
            }
            Print::Debug("删除场景数据成功.");
        }
        catch (exception e)
        {
            Print::Debug("删除场景数据时出错...(处于开发者模式时将显示错误信息)");
            Print::Exception(e.what());
        }
    }
};

class Scene
{
private:
    bool first = true;
    bool onload = false;
    bool fixed_onload = false;
    int scene_count = 0;
    int last_scene_count = 0;
private:
    vector<void*> scenes;
private:
    typedef Scene_Loading sloading;
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
            fixed_onload = true;
        }
    }
public:
    void Draw()
    {
        if(onload)
            switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw();
            break;
        case 1:
            ((s1*)scenes[1])->Draw();
            break;
        case 2:
            ((sloading*)scenes[2])->Draw();
            break;
        default:
            break;
        }
    }

    void Draw_Shadow()
    {
        if (onload)switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_Shadow();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_Shadow();
            break;
        case 2:
            ((sloading*)scenes[2])->Draw_Shadow();
            break;
        default:
            break;
        }
    }

    void Draw_SSAO()
    {
        if (onload)switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_SSAO();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_SSAO();
            break;
        case 2:
            ((sloading*)scenes[2])->Draw_SSAO();
            break;
        default:
            break;
        }
    }

    void Draw_UI()
    {
        if (onload)switch (scene_count)
        {
        case 0:
            ((s0*)scenes[0])->Draw_UI();
            break;
        case 1:
            ((s1*)scenes[1])->Draw_UI();
            break;
        case 2:
            ((sloading*)scenes[2])->Draw_UI();
            break;
        default:
            break;
        }
    }
    void FixedUpdate()
    {
        if (fixed_onload)
        {
            switch (scene_count)
            {
            case 0:
                ((s0*)scenes[0])->FixedUpdate();
                break;
            case 1:
                ((s1*)scenes[1])->FixedUpdate();
                break;
            case 2:
                ((sloading*)scenes[2])->FixedUpdate();
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
        case 2:
            scenes[_count] = new sloading();
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
        case 2:
            delete (sloading*)scenes[_count];
            break;
        }
        scenes[_count] = NULL;
    }

    void Delete()
    {
        onload = false;
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
            if (scene_count == 2)
            {
                fixed_onload = false;
                last_scene_count = scene_count;
                onload = false;
                ClearEvent();
                scene_count = _count;
                New_Scene(_count);
                onload = true;
                Delete_Scene(last_scene_count);
                fixed_onload = true;
            }
            else
            {
                fixed_onload = false;
                last_scene_count = scene_count;
                onload = false;
                ClearEvent();
                scene_count = 2;
                New_Scene(2);
                ((sloading*)(Get_scene()))->NextScene(_count);
                onload = true;
                Delete_Scene(last_scene_count);
                //fixed_onload = true;
            }
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
        case 2:
            ((sloading*)scenes[scene_count])->UI_Count(_count);
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
        case 2:
            fonts = &(((sloading*)scenes[scene_count])->fonts);
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
        Print::Debug("未找到相应的fonts UI对象");
    }
    private:
        int changing_scnen_count = -1;
        void ChangeScene()
        {

        }
};