#pragma once
//将UTF8编码格式转化为GBT32
//string UTF8ToGB(const char* str);
//将string类型转化为float类型
//float StringTofloat(string s);

class Model
{
private:
    string model_type[5] = { "", "", "", "", "" };
    string texture_path[5] = { "", "", "", "", "" };//纹理路径
    int vertices_length = 0;
    GLfloat* vertices;//模型数据
private:
    bool first = true;
    string XTpath = "";
    string Xpath = "";
    GLuint vao = 0, vbo = 0;
    GLuint texture0 = 0, texture1 = 0, texture2 = 0, texture3 = 0;
    Shader shader;
public:
    //判断是否为单独的模型对象
    bool alone = true;
    //当前模型的位置
    vec3 position;
public:
    bool rigid_active;
    float rigidBody;
    string collider_main_type;//碰撞体类型
    string collider_attack_type;//碰撞体类型
    vec3 collider_main_lwh;
    float collider_attack_radius;
    bool collider_main_active = false, collider_attack_active = false;//是否启用碰撞体
    vec3 collider_main_point, collider_attack_point;//相对位置
    vec3 collider_main_point_absolute, collider_attack_point_absolute;//绝对位置
public:
    Model()
    {
        first = true;
        Print::Debug("创建了一个模型类...");
    }

    Model(string path)
    {
        Get_Xpath(path);
        Read(path);
    }

    ~Model()
    {
        try
        {
            Print::Debug("正在尝试删除动态创建的模型数据...");
            if (vertices != NULL)
                delete[] vertices;
        }
        catch (exception e)
        {
            Print::Debug("删除动态创建的模型数据时出现错误：");
            Print::Debug(e.what());
        }
    }

    string Get_name()
    {
        return texture_path[0];
    }

    void Get_Xpath(string _path)
    {
        //获取到最后一个斜杠的位置
        int a = 0, b = 0;
        for (int i = 0; i < _path.length(); i++)
        {
            if (_path.substr(i, 1) == "/")
            {
                a = b;
            }
            b++;
        }
        XTpath = _path;
        Xpath = _path.substr(0, a + 1);
        Print::Debug("ModelX文件位置：" + Xpath);
    }

    void First()
    {
        if (first)
        {
            Print::Debug("当前模型位置:" + Xpath);
            Print::Debug("加载模型顶点...");
            load_vao(&vao, &vbo);
            Print::Debug(to_string(sizeof(vertices)));
            Print::Debug("加载模型渲染参数...");
            load_vbo(vertices, sizeof(GLfloat) * vertices_length);
            shader = Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
            Load_shader(shader);
            Print::Debug("加载模型贴图...");
            string t1 = "", t2 = "", t3 = "", t4 ="";
            if (texture_path[0] != "")t1 = Xpath + texture_path[0];
            if (texture_path[1] != "")t2 = Xpath + texture_path[1];
            if (texture_path[2] != "")t3 = Xpath + texture_path[2];
            if (texture_path[3] != "")t4 = Xpath + texture_path[3];
            Load_texture(&texture0, &texture1, &texture2, &texture3, t1, t2, t3, t4);
            Print::Debug("模型初始化完成.");
            Print::Debug(XTpath);
            first = false;
        }
    }

    //作为单独模型时
    void Draw(vec3 _position)
    {
        if (first && alone)position = _position;
        First();
        Load_shader_u(shader, true);
        Load_matrix_model(&Model_Mat4, position, vec3(1.0f));
        Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
    }
    void Draw_Collider(vec3 _position)
    {
        if (first && alone)position = _position;
        First();
        if (Math::Distance_max(position, PlayerPos) < 10)
        {
            if (collider_main_active)
            {
                collider_main_point_absolute = collider_main_point + position;
                //创建碰撞体，并添加到“实体池”中去
                Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh);//获取合适位置的碰撞体
                CCo->type = collider_main_type;
                Collider_Layer_Default.collection_entity.push_back(CCo);
            }
            //如果具有重量
            if (rigidBody > 0.0f)
            {
                Physics_Gravity(this, DeltaTime);
            }
            if (collider_attack_active)
            {
                collider_attack_point_absolute = collider_attack_point + position;
                Collider_Sphere* CS = new Collider_Sphere(collider_attack_point_absolute, collider_attack_radius);
                CS->type = collider_attack_type;
                Collider_Layer_Default.collection_energy.push_back(CS);
            }
        }
    }
    //重力检测(仅仅用于自主判断)
    static void Physics_Gravity(Model* model_p, float deletatime)
    {
        if (model_p->rigid_active)
        {
            model_p->rigid_active = true;
            model_p->rigidBody = model_p->rigidBody;
            //Print::Debug("重力检测");
            if (model_p->collider_main_active)
            {
                //获取到碰撞体基于世界坐标系的位置
                vec3 poi = model_p->collider_main_point_absolute;
                //将下一步位置先一部求出，进行预判断,误差范围0.01f
                poi.y -= model_p->rigidBody * deletatime + 0.01f;
                vec3 lwh = model_p->collider_main_lwh;
                //还原当前模型的碰撞体
                Collider_Coboid* cco = new Collider_Coboid(poi, lwh);
                //获得离当前模型碰撞体最近的碰撞体
                Collider_Coboid* ncco = Collider_Layer_Default.Recent_Collision_Entity(poi);
                //Print::Debug("重力检测");
                if (Collider_Layer_Default.Recent_Collision_Entity(poi) != 0)
                {
                    bool a = Collider_Coboid::Collision(cco, ncco);
                    if (!a)
                    {
                        model_p->position.y -= model_p->rigidBody * deletatime;
                    }
                    else
                    {
                        float difference = model_p->position.y - model_p->collider_main_point_absolute.y;
                        //获得最近的坐标
                        model_p->position.y = ncco->Point().y + ((ncco->Width() + cco->Width()) * 0.5f) + difference;
                        //Print::Debug(to_string(model_p->position.y));
                    }
                }
                //删除new出来的对象
                delete cco;
            }
        }
    }

    void Draw(vec3* _position)
    {
        position = *_position;
        First();
        Load_shader_u(shader, true);
        Load_matrix_model(&Model_Mat4, position, vec3(1.0f));
        Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
        *_position = position;
    }
    void Draw_Collider(vec3* _position)
    {
        First();
        position = *_position;
        if (Math::Distance_max(position, PlayerPos) < 10)
        {
            if (collider_main_active)
            {
                //得到碰撞体绝对位置
                collider_main_point_absolute = collider_main_point + position;
            }
            //如果具有重量
            if (rigidBody > 0.0f)
            {
                Physics_Gravity(this, DeltaTime);
            }
            if (collider_attack_active)
            {
                //得到攻击碰撞体绝对位置
                collider_attack_point_absolute = collider_attack_point + position;
            }
        }
    }
    //不对类对象位置值进行改变,直接绘制
    void Draw(vec3 _position, bool trueX)
    {
        First();
        Load_shader_u(shader, true);
        Load_matrix_model(&Model_Mat4, _position, vec3(1.0f));
        Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
    }
    void Draw_Collider(vec3 _position, bool trueX)
    {
        First();
        if (Math::Distance_max(_position, PlayerPos) < 10)
        {
            if (collider_main_active)
            {
                //得到碰撞体绝对位置
                vec3 cmpa = collider_main_point + _position;
                //创建碰撞体，并添加到“实体池”中去
                Collider_Coboid* CCo = new Collider_Coboid(cmpa, collider_main_lwh);//获取合适位置的碰撞体
                CCo->type = collider_main_type;
                Collider_Layer_Default.collection_entity.push_back(CCo);
            }
            //如果具有重量
            if (rigidBody > 0.0f)
            {
                Physics_Gravity(this, DeltaTime);
            }
            if (collider_attack_active)
            {
                //得到攻击碰撞体绝对位置
                vec3 capa = collider_attack_point + _position;
                Collider_Sphere* CS = new Collider_Sphere(capa, collider_attack_radius);
                CS->type = collider_attack_type;
                Collider_Layer_Default.collection_energy.push_back(CS);
            }
        }
    }

    void Draws(vec3 _position, bool trueX)
    {
        //if (distance(position.x, PlayerPos.x) < 10 && distance(position.z, PlayerPos.z) < 10)
        //{
        //    if (collider_main_active)
        //    {
        //        collider_main_point_absolute = collider_main_point + position;
        //        //创建碰撞体，并添加到“实体池”中去
        //        Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh);//获取合适位置的碰撞体
        //        CCo->type = collider_main_type;
        //        collection_entity.push_back(CCo);
        //    }
        //    //如果具有重量
        //    if (rigidBody > 0.0f)
        //    {
        //        Physics_Gravity(this, DeltaTime);
        //    }
        //    if (collider_attack_active)
        //    {
        //        collider_attack_point_absolute = collider_attack_point + position;
        //        Collider_Sphere* CS = new Collider_Sphere(collider_attack_point_absolute, collider_attack_radius);
        //        CS->type = collider_attack_type;
        //        collection_energy.push_back(CS);
        //    }
        //}
        //Load_shader_u(shader, true);
        Load_matrix_model(&Model_Mat4, _position);
        shader.use();
        shader.setMat4("projection", Projection_Mat4);
        shader.setMat4("view", View_Mat4);
        shader.setMat4("model", Model_Mat4);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
    }

    //绘制多个模型(位置数组，数量)
    void Draws(vec3* _position, int count)
    {
        shader.use();
        shader.setMat4("projection", Projection_Mat4);
        shader.setMat4("view", View_Mat4);
        for (int i = 0; i < count; i++)
        {
            Load_matrix_model(&Model_Mat4, _position[i]);
            shader.setMat4("model", Model_Mat4);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
    }

    void Draw(vec3 _position, vec3 _scale)
    {
        if (first && alone)position = _position;
        First();
        Load_shader_u(shader, true);
        Load_matrix_model(&Model_Mat4, position, _scale);
        Load_draw(&vao, &texture0, &texture1, &texture2, &texture3, shader);
        //Print::Debug("???f");
    }
    //代码待修改
    void Draw_Collider(vec3 _position, vec3 _scale)
    {
        if (first && alone)position = _position;
        First();
        //Print::Debug("???x");
        if (collider_main_active)
        {
            collider_main_point_absolute = collider_main_point + _position;
            //创建碰撞体，并添加到“实体池”中去
            Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh * _scale);//获取合适位置的碰撞体
            CCo->type = collider_main_type;
            Collider_Layer_Default.collection_entity.push_back(CCo);
        }
        //Print::Debug("???y");
        //如果具有重量
        if (rigidBody > 0.0f)
        {
            Physics_Gravity(this, DeltaTime);
        }
        //Print::Debug("???z");
        if (collider_attack_active)
        {
            collider_attack_point_absolute = collider_attack_point + _position;
            Collider_Sphere* CS = new Collider_Sphere(collider_attack_point_absolute, collider_attack_radius);
            CS->type = collider_attack_type;
            Collider_Layer_Default.collection_energy.push_back(CS);
        }
    }
    void ChangeCollider(vec3 _position)
    {
        if (collider_main_active)
        {
            collider_main_point_absolute = collider_main_point + _position;
        }
        //如果具有重量
        if (rigidBody > 0.0f)
        {
            //Physics_Gravity(this, DeltaTime);
        }
        if (collider_attack_active)
        {
            collider_attack_point_absolute = collider_attack_point + _position;
        }
    }

    void Draw_Shadow(vec3 _position)
    {
        First();
        glm::mat4 _model = glm::mat4(1.0f);
        Load_matrix_model(&_model, _position);
        Open_Shadow_SetShaderMat4("model", _model);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    void Draw_SSAO(vec3 _position, Shader& _shader)
    {
        First();
        glm::mat4 _model = glm::mat4(1.0f);
        Load_matrix_model(&_model, _position);
        shaderGeometryPass_SSAO.use();
        shaderGeometryPass_SSAO.setMat4("model", _model);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Position(vec3 _position)
    {
        position = _position;
    }

    void Read(string path)
    {
        Print::Debug("modelXXXIIIFile" + path);
        //逐行读取
        ifstream infile;
        infile.open(path.data());   //将文件流对象与文件连接起来 
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

        string s = "";
        bool active_wirte = false;
        while (getline(infile, s))
        {
            s = UTF8ToGB(s.c_str());
            //count << s << endl;
            if (active_wirte)
            {
                Print::Debug("modelXXX" + s);
                if (Data(s))
                {
                    active_wirte = false;
                }
            }
            else if (Line(s))
            {
                active_wirte = true;
            }
        }
        infile.clear();
        infile.close();             //关闭文件输入流 
    }

    //识别类型
    bool Line(string s)
    {
        //如果第一行为#，则代表该行为注释
        if (s.substr(0, 1) == "#")
        {

        }
        else if (s.length() > 4 + 1)
        {
            if (s.substr(0, 4) == "版本")
            {
                model_type[0] = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "类型")
            {
                model_type[1] = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "格式")
            {
                model_type[2] = s.substr(4 + 1);
            }
            else if (s.length() > 6 + 1)
            {
                if (s.substr(0, 6) == "碰撞体")
                {
                    if (s.substr(6 + 1, 8) == "球体攻击")
                    {
                        string pengzhuangti = s.substr(6 + 1 + 8);
                        string radius_s = "";//碰撞体宽度(半径)
                        int pointc = 0;//vec3计数
                        vec3 point3;//坐标点
                        string point = "";
                        for (int i = 0; i < pengzhuangti.length(); i++)
                        {
                            string is = pengzhuangti.substr(i, 1);
                            if (point == "")
                            {
                                //未到坐标数据时
                                if (is != "(")
                                {
                                    radius_s.append(is);
                                }
                                else
                                {
                                    point = "(";
                                }
                            }
                            else
                            {
                                if (point == "(")
                                {
                                    point = is;
                                }
                                else if (is == ")")
                                {
                                    point3[pointc] = stringTofloat(point);
                                    pointc++;
                                }
                                else if (is == ",")
                                {
                                    point3[pointc] = stringTofloat(point);
                                    pointc++;
                                    point = "(";
                                }
                                else
                                {
                                    point.append(is);
                                }
                            }
                        }
                        float rad = stringTofloat(radius_s);
                        collider_attack_point = point3;
                        collider_attack_radius = rad;
                        collider_attack_active = true;
                    }
                    else if(s.substr(6 + 1, 8) == "主要方块")
                    {
                        string pengzhuangti = s.substr(6 + 1 + 8);
                        string lwh_s = "";//碰撞体长宽高
                        int lwhc = 0;
                        vec3 lwh;
                        int pointc = 0;//vec3计数
                        vec3 point3;//坐标点
                        string point = "";
                        for (int i = 0; i < pengzhuangti.length(); i++)
                        {
                            string is = pengzhuangti.substr(i, 1);
                            if (point == "")
                            {
                                //未到坐标数据时
                                if (is == "(")
                                {
                                    point = "(";
                                    lwh[lwhc] = stringTofloat(lwh_s);
                                    lwh_s = "";
                                    lwhc++;
                                }
                                else if (is == ",")
                                {
                                    lwh[lwhc] = stringTofloat(lwh_s);
                                    lwh_s = "";
                                    lwhc++;
                                }
                                else
                                {
                                    lwh_s.append(is);
                                }
                            }
                            else
                            {
                                if (point == "(")
                                {
                                    point = is;
                                }
                                else if (is == ")")
                                {
                                    point3[pointc] = stringTofloat(point);
                                    pointc++;
                                }
                                else if (is == ",")
                                {
                                    point3[pointc] = stringTofloat(point);
                                    pointc++;
                                    point = "(";
                                }
                                else
                                {
                                    point.append(is);
                                }
                            }
                        }
                        collider_main_point = point3;
                        collider_main_lwh = lwh;
                        collider_main_active = true;
                    }
                    else if (s.substr(6 + 1, 8) == "作为实体")
                    {
                        if (s.substr(6 + 1 + 8, 8) != "作为实体")
                            rigidBody = stringTofloat(s.substr(6 + 1 + 8 + 1));//如果后面有数字则证明具有重力
                        rigid_active = true;
                    }
                    else if (s.substr(6 + 1, 8) == "碰撞类型")
                    {
                        if (s.substr(6 + 1 + 8 + 1, 8) != "球体攻击")
                            collider_attack_type = s.substr(6 + 1 + 8 + 1 + 8 + 1);
                        if (s.substr(6 + 1 + 8 + 1, 8) != "主要方块")
                            collider_main_type = s.substr(6 + 1 + 8 + 1 + 8 + 1);

                    }
                }
                else if (s.substr(0, 6) == "顶点数") 
                {
                    vertices_length = 8 * atoi(s.substr(6 + 1).c_str());
                    //动态创建一个数组
                    vertices = new GLfloat[vertices_length];
                    Print::Debug("动态数组大小:" + to_string(vertices_length));
                }
                else if (s.substr(0, 6) == "漫反射")
                {
                    texture_path[0] = s.substr(6 + 1);
                    Print::Debug("漫反射:" + texture_path[0]);
                }
                else if (s.length() > 8 + 1)
                {
                    if (s.substr(0, 8) == "镜面反射")
                    {
                        texture_path[1] = s.substr(8 + 1);
                    }
                    else if (s.substr(0, 8) == "环境反射")
                    {
                        texture_path[2] = s.substr(8 + 1);
                        Print::Debug("环境反");
                        Print::Debug("环境反射:" + texture_path[2]);
                    }
                    else if (s.substr(0, 8) == "法线贴图")
                    {
                        texture_path[3] = s.substr(8 + 1);
                    }
                }
            }

        }
        else if (s.substr(0, 4) == "数据")
        {
            Print::Debug(s.substr(0, 4) + "model" + s);
            return true;
        }
        return false;
    }

    int count = 0;
    //读取数据
    bool Data(string s) 
    {
        //读取到注释时跳过
        if (s.substr(0, 1) == "#")
        {
            return false;
        }
        camma(s.substr(1));
        if (count >= vertices_length)
        {
            Print::Debug(to_string(count));
            count = 0;
            return true;
        }
        return false;
    }

    //读取每一个逗号前的小数据并存入数组中
    void camma(string s)
    {
        string x = "";
        for (int i = 0; i < s.length(); i++)
        {
            if (s.substr(i,1) == ",")
            {
                vertices[count] = stringTofloat(x);
                count++;
                x = "";
            }
            else
            {
                x.append(s.substr(i, 1));
            }
        }
    }

    //模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）  
    template <class Type>
    Type stringToNum(const string& str)
    {
        istringstream iss(str);
        Type num;
        iss >> num;
        return num;
    }

    static void load_vao(GLuint* VAO, GLuint* VBO)
    {
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);

        //绑定顶点数组对象
        glBindVertexArray(*VAO);
        //绑定顶点缓冲对象
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    }

    //加载顶点数组位置属性(顶点数组， 长度)
    static void load_vbo(float vertices[], int lengthCout)
    {
        //创建并初始化缓冲区对象的数据存储
        glBufferData(GL_ARRAY_BUFFER, lengthCout, vertices, GL_STATIC_DRAW);

        //对顶点缓冲对象内容进行解析定义
        // -----------------------------
        //位置属性
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);//启用顶点属性0
        //法向量属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //纹理坐标属性
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);//启用顶点属性1
    }

    public:
        vec3 Collider_main_point()
        {
            return collider_main_point;
        }

        vec3 Collider_attack_point()
        {
            return collider_attack_point;
        }

        vec3 Collider_main_lwh()
        {
            return collider_main_lwh;
        }

        float Collider_attack_radius()
        {
            return collider_attack_radius;
        }
};
//string Model_Mat4::model_type = "";
//GLfloat* Model_Mat4::vertices = NULL;

