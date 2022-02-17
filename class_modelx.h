#pragma once
class ModelX
{
private:
    struct Type {
        string vision;
        string leixing;
        int zhenlv = 1;
    };
    Type _type;
private:
    bool first = true;
    int point = 0;//动画位置
    float time = 0.0f;
    string Xpath = "";//当前模型X文件的相对位置:如Path_Model + Xpath
    string name_lastAnimation = "";//上一帧动画名称
    int cout_animations = 0;
    string* animations_name;
    string** animations_path;
    vector<vector<Model*>*> mods_x0;
private:
    bool rigid_active;    //是否激活刚体
    float rigidBody;
public:
    vec3 position;
public:
    ModelX()
    {

    }

    ModelX(string path)
    {
        Get_Xpath(path);
        Read(path);
    }

    ~ModelX()
    {
        try
        {
            Print::Line("正在尝试删除动态创建的模型对象...");
            Delete_Model();
            Print::Debug("动画路径数量:" + to_string(cout_animations));
            Print::Debug("动画名称:" +  *animations_name);
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_path[i] != NULL)
                    delete[] animations_path[i];
            }
            Print::Debug("???");
            delete[] animations_name;
        }
        catch (exception e)
        {
            Print::Line("删除动态创建的模型时出现错误：");
            Print::Line(e.what());
        }
    }
    /// <summary>
    /// 获取模型各个文件的位置
    /// </summary>
    /// <param name="path">X文件的路径</param>
    void Get_Xpath(string path)
    {
        //获取到最后一个斜杠的位置
        int a = 0, b = 0;
        for (int i = 0; i < path.length(); i++)
        {
            if (path.substr(i, 1) == "/")
            {
                a = b;
            }
            b++;
        }
        Xpath = path.substr(0, a + 1);
        Print::Line("ModelX文件位置：" + Xpath);
    }
    /// <summary>
    /// 数据初始化
    /// </summary>
    void First()
    {
        if (first)
        {
            Load_Model();
            first = false;
        }
    }
    /// <summary>
    /// 数据初始化(改变动画引用)
    /// </summary>
    /// <param name="name_animation">动画名</param>
    void First(string name_animation)
    {
        First();
        //如果动画改变
        if (name_lastAnimation != name_animation)
        {
            //判断动画位置
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_name[i] == name_animation)
                {
                    point = i;
                }
            }
            //时间重置
            time = 0;
        }
        name_lastAnimation = name_animation;
    }
    /*
    模型坐标仅初始化一次
    */
    void Draw(vec3 _position)
    {
        if (first)position = _position;
        First();
        //绘制当前帧模型
        (*GetAnimations(point))[GetCurrentFrame()]->Draw(position);
        if (time > 1)
        {
            time = 0;
        }
    }
    void Draw_Collider(vec3 _position)
    {
        if (first)position = _position;
        First();
        time += FixedDeltaTime;
        //获得与当前时间最接近的帧
        int cur = 0;
        int zhi = 1000000;
        float timeCha = 0.0f;
        float  timeCha0 = 1.0f;
        float x = 0.0;
        for (int i = 0; i < _type.zhenlv; i++)
        {
            x = i;
            timeCha = Distance_PtoP(time, x / _type.zhenlv);
            if (timeCha < timeCha0)
            {
                cur = i;
                timeCha0 = timeCha;
            }
        }
        Physics_Check((*GetAnimations(point))[cur], FixedDeltaTime, &position);
    }
    /*
    模型坐标按照位置进行渲染，并为传入参数赋予当前模型的渲染后的位置值
    */
    void Draw(vec3* _position)
    {
        //position = *_position;
        First();
        //绘制当前帧模型
        (*GetAnimations(point))[GetCurrentFrame()]->Draw(_position);
        if (time > 1)
        {
            time = 0;
        }
    }
    void Draw_Collider(vec3* _position)
    {
        First();
        time += FixedDeltaTime;
        //获得与当前时间最接近的帧
        int cur = 0;
        int zhi = 1000000;
        float timeCha = 0.0f;
        float  timeCha0 = 1.0f;
        float x = 0.0;
        for (int i = 0; i < _type.zhenlv; i++)
        {
            x = i;
            timeCha = Distance_PtoP(time, x / _type.zhenlv);
            if (timeCha < timeCha0)
            {
                cur = i;
                timeCha0 = timeCha;
            }
        }
        Physics_Check((*GetAnimations(point))[GetCurrentFrame()], FixedDeltaTime, _position);
        *_position = position;
    }
    /*
    模型坐标按照位置进行渲染，并为传入参数赋予当前模型的渲染后的位置值(动画基于数组中的位置，位置)
    */
    void Draw(int _point, vec3* _position)
    {
        point = _point;
        position = *_position;
        First();
        Draw(_position);
    }
    /*
    模型坐标仅初始化一次(动画名称，位置)
    */
    void Draw(string name_animation, vec3 _position)
    {
        if (first)position = _position;
        First(name_animation);
        Draw(_position);
    }
    /*
    模型坐标按照位置进行渲染，并为传入参数赋予当前模型的渲染后的位置值(动画名称，位置)
    */
    void Draw(string name_animation, vec3* _position)
    {
        position = *_position;
        First(name_animation);
        Draw(_position);
    }
    void ChangePosition(vec3* _position)
    {
        Physics_Check((*GetAnimations(point))[GetCurrentFrame()], FixedDeltaTime, _position);
        *_position = position;
    }
    void Draw_Shadow(vec3 _position)
    {
        First();
        //绘制当前帧模型
        (*GetAnimations(point))[GetCurrentFrame()]->Draw_Shadow(_position);
    }
    void Draw_SSAO(vec3 _position)
    {
        First();
        //绘制当前帧模型
        (*GetAnimations(point))[GetCurrentFrame()]->Draw_SSAO(_position, shaderGeometryPass_SSAO);
    }
    void Draw_SSAO(vec3 _position, Shader& _shader)
    {
        First();
        //绘制当前帧模型
        (*GetAnimations(point))[GetCurrentFrame()]->Draw_SSAO(_position, _shader);
    }
    //获取当前帧
    int GetCurrentFrame()
    {
        //获得与当前时间最接近的帧
        int cur = 0;
        int zhi = 1000000;
        float timeCha = 0.0f;
        float  timeCha0 = 1.0f;
        float x = 0.0;
        for (int i = 0; i < _type.zhenlv; i++)
        {
            x = i;
            timeCha = Distance_PtoP(time, x / _type.zhenlv);
            if (timeCha < timeCha0)
            {
                cur = i;
                timeCha0 = timeCha;
            }
        }
        return cur;
    }
    //求出点到点之间的距离
    float Distance_PtoP(float a, float b)
    {
        float c = a - b;
        if (c < 0)
        {
            c = b - a;
        }
        return c;
    }
    void Read(string path)
    {
        //逐行读取
        ifstream infileX;
        infileX.open(path.data());   //将文件流对象与文件连接起来 
        assert(infileX.is_open());   //若失败,则输出错误消息,并终止程序运行 

        string s = "";
        bool active_wirte = false;
        while (getline(infileX, s))
        {
            s = UTF8ToGB(s.c_str());
            //cout << s << endl;
            if (active_wirte)
            {
                Print::Line("modelXXX" + s);
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
        infileX.clear();
        infileX.close();             //关闭文件输入流 
    }
    string cur_name = "";
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
                _type.vision = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "类型")
            {
                _type.leixing = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "刚体")
            {
                rigidBody = stringTofloat(s.substr(4 + 1));
                rigid_active = true;
            }
            else if (s.substr(0, 4) == "帧率")
            {
                _type.zhenlv = atoi(s.substr(4 + 1).c_str());
            }
            else if (s.substr(0, 4) == "数量")
            {
                cout_animations = atoi(s.substr(4 + 1).c_str());
                animations_path = new string * [cout_animations];
                animations_name = new string[cout_animations];
                for (int i = 0; i < cout_animations; i++)
                {
                    //将每一个数据初始化
                    animations_name[i] == "";
                    //mods[i]->reserve(_type.zhenlv);
                }
            }
            else if (s.substr(0, 4) == "动画")
            {
                Print::Line(s.substr(0, 4) + ":::" + s);
                for (int i = 0; i < cout_animations; i++)
                {
                    if (animations_name[i] == "")
                    {
                        //存入动画名
                        cur_name = s.substr(4 + 1);
                        animations_name[i] = cur_name;
                        Print::Line("存入的动画名：" + animations_name[i]);
                        animations_path[i] = new string[_type.zhenlv];
                        //动态创建模型数组
                        //mods[i] = new Model[_type.zhenlv];
                        //扩容VECTOR对象
                        //mods[i].reserve(_type.zhenlv);
                    }
                }
                return true;
            }
            else if (s.length() > 6 + 1)
            {

            }

        }
        return false;
    }
    int cout = 0;
    //读取数据
    bool Data(string s)
    {
        //读取到注释时跳过
        if (s.substr(0, 1) == "#")
        {
            return false;
        }
        else if (s.substr(0, 1) == "A")
        {
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_name[i] == cur_name)
                {
                    //获取到需要填入的帧数
                    int x = atoi(s.substr(2, 1).c_str()) - 1;
                    if (cout == x)
                    {
                        animations_path[i][cout] = GetLast(s);
                        Print::Line("???" + to_string(i) + "???" + to_string(x) + "???cout" + to_string(cout) + animations_path[i][cout]);
                        cout++;
                    }
                    else
                    {
                        //将未填充的帧数给补齐
                        for (int ii = 0; ii < (x - cout + 1); ii++)
                        {
                            if (cout + ii == x)
                            {
                                animations_path[i][cout + ii] = GetLast(s);
                                Print::Line("???" + to_string(i) + "???" + to_string(x) + "???" + to_string(cout + ii) + animations_path[i][cout + ii]);
                            }
                            else
                            {
                                //使用前面已填充的帧补齐
                                animations_path[i][cout + ii] = animations_path[i][cout + ii - 1];
                                Print::Line("???" + to_string(i) + "???" + to_string(x) + "???" + to_string(cout + ii) + animations_path[i][cout + ii]);
                            }
                        }
                        cout = x;
                    }
                    break;
                }
            }
        }
        else if (s.substr(0, 4) == "动画")
        {
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_name[i] == cur_name)
                {
                    //将未填充的帧数给补齐
                    for (int ii = 0; ii < (_type.zhenlv - cout); ii++)
                    {
                        //使用前面已填充的帧补齐
                        animations_path[i][cout + ii + 1] = animations_path[i][cout - 1];
                    }
                    cout = _type.zhenlv;
                    break;
                }
            }
            Print::Line(s.substr(0, 4) + ":::" + s);
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_name[i] == "")
                {
                    //存入动画名
                    cur_name = s.substr(4 + 1);
                    animations_name[i] = cur_name;
                    animations_path[i] = new string[_type.zhenlv];
                    //动态创建模型数组
                    //mods[i] = new Model[_type.zhenlv];
                    //扩容VECTOR对象
                    //mods[i].reserve(_type.zhenlv);
                }
            }
            cout = 0;
            return true;
        }
        else if (s.substr(0, 1) == "E")
        {
            for (int i = 0; i < cout_animations; i++)
            {
                if (animations_name[i] == cur_name)
                {
                    //将未填充的帧数给补齐
                    for (int ii = 0; ii < (_type.zhenlv - cout - 1); ii++)
                    {
                        //使用前面已填充的帧补齐
                        animations_path[i][cout + ii + 1] = animations_path[i][cout + ii];
                        Print::Debug("???" + to_string(i) + "???" + to_string(_type.zhenlv) + "???" + to_string(cout + ii + 1) + animations_path[i][cout + ii + 1]);
                    }
                    cout = _type.zhenlv - 1;
                    break;
                }
            }
            cout = 0;
            return true;
        }
        if (cout >= _type.zhenlv)
        {
            cout = 0;
            return true;
        }
        return false;
    }
    //载入模型
    void Load_Model()
    {
        for (int i = 0; i < cout_animations; i++)
        {
            vector<Model*>* x = new vector<Model*>;
            for (int ii = 0; ii < _type.zhenlv; ii++)
            {
                if (ii != 0)
                {
                    if (animations_path[i][ii - 1] == animations_path[i][ii])
                    {
                        Print::Line(animations_path[i][ii - 1] + "???XXX???" + Xpath + animations_path[i][ii]);
                        x->push_back((*x)[ii - 1]);
                    }
                    else
                    {
                        Model* model = new Model(Xpath + animations_path[i][ii]);
                        Print::Line("???XXX???" + Xpath + animations_path[i][ii]);
                        model->alone = false;
                        x->push_back(model);
                    }
                }
                else
                {
                    Model* model = new Model(Xpath + animations_path[i][ii]);
                    Print::Line("???XXX???" + Xpath + animations_path[i][ii]);
                    model->alone = false;
                    x->push_back(model);
                }
            }
            mods_x0.push_back(x);
            Print::Line("???XXX???" + to_string(x->size()));
        }
    }
    //删除模型
    void Delete_Model()
    {
        for (int i = 0; i < cout_animations; i++)
        {
            try 
            {
                if (mods_x0.size() > i)
                {
                    vector<Model*>* x = mods_x0[i];
                    if (x != NULL)
                    {
                        for (int ii = 0; ii < x->size(); ii++)//_type.zhenlv
                        {
                            if (ii != 0)
                            {
                                if (animations_path[i][ii - 1] == animations_path[i][ii])
                                {
                                    //Model* m = (*x)[ii];
                                    //delete m;
                                }
                                else
                                {
                                    Model* m = (*x)[ii];
                                    Print::Debug("delete m;");
                                    delete m;
                                }
                            }
                            else
                            {
                                Model* m = (*x)[ii];
                                Print::Debug("delete m;");
                                delete m;
                            }
                        }
                        x->clear();
                        Print::Debug("x->clear()");
                        delete x;
                        Print::Debug("???XXX???" + to_string(x->size()));
                    }
                }
            }
            catch (exception e)
            {
                Print::Debug(e.what());
            }
        }
        mods_x0.clear();
    }
    vector<Model*>* GetAnimations(int i)
    {
        return mods_x0[i];
    }
    //获取冒号后的值
    string GetLast(string s)
    {
        for (int i = 0; i < s.length(); i++)
        {
            if (s.substr(i, 1) == ":")
            {
                return s.substr(i + 1);
            }
        }
        return 0;
    }

    void Physics_Check(Model* model, float _deletatime, vec3* _position)
    {
        bool _collision = false;
        int final_v = 0;
        ////归一化后的碰撞体计算
        ////--------------------
        //vec3 xiangliang = -(*_position - position);//用预到达位置减去现有位置，得到方向向量
        //xiangliang = normalize(xiangliang);//将向量归一化
        //vec3 poi_n = model->collider_main_point_absolute;
        //vec3 lwh_n = model->collider_main_lwh;
        //poi_n += xiangliang * 0.01f;
        //Collider_Coboid* cco_n = new Collider_Coboid(poi_n, lwh_n);
        //vector<Collider_Coboid*> nccos_n = Collider_Layer_Default.Recent_Collision_Entity(poi_n, 50);
        //直接的位置运动计算碰撞体
        //------------------------
        vec3 poi = model->collider_main_point_absolute;
        vec3 lwh = model->collider_main_lwh;
        poi += *_position - position;
        Collider_Coboid* cco = new Collider_Coboid(poi, lwh);
        vector<Collider_Coboid*> nccos = Collider_Layer_Default.collection_entity;//Collider_Layer_Default.Recent_Collision_Entity(poi, 50);
        //碰撞检测及位置改变
        //-------------------
        float _yaw = Yaw, _pitch = 0.0f;//偏航角，俯仰角
        float _yaw_cout = 0.0f;
        glm::vec3 _front;
        float _distance = distance(*_position, position);
        vec3 _next_position = *_position;
        //vec3 _next_position_n = *_position * 0.1f;
        vec3 _next_position_x = vec3(_position->x, position.y, position.z);
        vec3 _next_position_y = vec3(position.x, _position->y, position.z);
        vec3 _next_position_z = vec3(position.x, position.y, _position->z);
        vec3 _next_position_final = position;
        float _next_y = 1.0f;
        if (nccos.size() > 0)
        {
            //for (int v = 0; v < 20; v++)
            //{
            //    for (int i = 0; i < nccos.size(); i++)
            //    {
            //        //如果发生碰撞
            //        if (Physics_Math(cco, nccos[i], model, _deletatime, 0, &_next_position, 1))// && Physics_Math(cco_n, nccos[i], model, _deletatime, 0, &_next_position_n, 1)
            //        {
            //            _collision = true;
            //            vec3 v = (*nccos[i]).LWH();
            //            //判断碰撞体最低位置高度
            //            if ((*cco).LWH().b > v.b * 4 && (*cco).Point().y - (*cco).LWH().b > (*nccos[i]).Point().y - v.b)
            //            {
            //                PlayerJumpMin();
            //            }
            //        }
            //    }
            //    //当未发生碰撞，且方向与归一化后的位置一致时
            //    if (!_collision)
            //    {
            //        break;
            //    }
            //    else
            //    {
            //        if (v < 10)
            //            _next_position.y += _next_y * 0.01f;//DeltaTime;
            //            //Print::Debug(to_string(_next_position.y));
            //        if (v == 10)
            //        {
            //            _yaw = Yaw;
            //            _pitch = Pitch;
            //            _next_position = *_position;
            //        }
            //        if (v >= 10)
            //        {
            //            _yaw_cout = (10.0f) * 1;
            //            _yaw += _yaw_cout;
            //            //_pitch = (_pitch + 10.0f) * 1;
            //            if (_yaw - Yaw > 45.0f)
            //                _yaw = Yaw - 45.0f;
            //            else if (_yaw - Yaw < -45.0f)
            //                _yaw = Yaw + 45.0f;
            //            // make sure that when Pitch is out of bounds, screen doesn't get flipped
            //            if (_pitch > 89.0f)
            //                _pitch = -89.0f;
            //            else if (_pitch < -89.0f)
            //                _pitch = 89.0f;
            //            _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
            //            _front.y = sin(glm::radians(_pitch));
            //            _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
            //            _front = glm::normalize(_front);
            //            _next_position = position + (_distance * _front);
            //        }
            //        _next_position_n = *_position * 0.1f;
            //        cco->Point(cco->Point() + _next_position - position);
            //        //cco_n->Point(cco->Point() + _next_position_n - position);
            //    }
            //}
            //删除new出来的对象

            for (int x = 0; x < 3; x++)
            {
                if (x == 0)
                    _next_position = _next_position_x;
                else if (x == 1)
                    _next_position = _next_position_y;
                else if (x == 2)
                    _next_position = _next_position_z;
                for (int i = 0; i < nccos.size(); i++)
                {
                    cco->Point(_next_position);
                    //如果发生碰撞
                    if (Physics_Math(cco, nccos[i], model, _deletatime, 0, &_next_position, 1))// && Physics_Math(cco_n, nccos[i], model, _deletatime, 0, &_next_position_n, 1)
                    {
                        _collision = true;
                        //Print::Debug("移动时发生碰撞...");
                        vec3 v = (*nccos[i]).LWH();
                        if (x == 1)
                        {
                            //判断碰撞体最低位置高度
                            //(*cco).Point().y - ((*cco).LWH().b * 0.5f) < (*nccos[i]).Point().y - (v.b * 0.5f)
                            Print::Debug(to_string((*cco).LWH().y) + "??" + to_string(v.y));
                            Print::Debug(to_string((*cco).Point().y) + "??" + to_string((*nccos[i]).Point().y));
                            if ((*cco).Point().y - ((*cco).LWH().y * 0.25f) > (*nccos[i]).Point().y + (v.y * 0.5f))//如果当前模型高度大于碰撞体高度的4倍，且位置高于碰撞体，则向上跳跃(*cco).LWH().y > v.y * 4 && (*cco).Point().y > (*nccos[i]).Point().y
                            {
                                //PlayerJumpMin();
                                Print::Debug("碰撞位置给予改变...");
                                Collider_Coboid ncco(vec3(_next_position.x, (*cco).Point().y + (v.y * 0.6f), _next_position.z) , lwh);
                                bool iftrue = false;
                                for (int i = 0; i < nccos.size(); i++)
                                {
                                    //如果发生碰撞
                                    if (Physics_Math(&ncco, nccos[i], model, _deletatime, 0, &_next_position, 1))// && Physics_Math(cco_n, nccos[i], model, _deletatime, 0, &_next_position_n, 1)
                                    {
                                        Print::Debug("改变位置后仍然发生碰撞:" + to_string((*cco).Point().y + (v.y * 0.6f)));
                                        iftrue = true;
                                    }
                                }
                                if (!iftrue)
                                {
                                    _next_position.y = (*cco).Point().y + (v.y * 0.6f);
                                    cco->Point(_next_position);
                                    _collision = false;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (!_collision)
                {
                    if (x == 0)
                        position.x = _next_position.x;
                    else if (x == 1)
                        position.y = _next_position.y;
                    else if (x == 2)
                        position.z = _next_position.z;
                }
                _collision = false;
            }
            delete cco;
        }
        model->ChangeCollider(position);
        //if (!_collision)
        //{
        //    position = _next_position;//如果全部都未碰撞，则改变位置
        //    //Print::Debug("找到最后的" + to_string(position.x) + "|" + to_string(position.y) + "|" + to_string(position.z));
        //    model->ChangeCollider(position);
        //}
        Physics_Gravity(model, _deletatime, &position);
    }

    //重力检测
    void Physics_Gravity(Model* model_p,float _deletatime, vec3* _position)
    {
        if (rigid_active && rigidBody != 0.0)
        {
            //model_p->rigid_active = true;
            //model_p->rigidBody = rigidBody;
            //Print::Debug("重力检测");
            if (model_p->collider_main_active)
            {
                //获取到碰撞体基于世界坐标系的位置
                vec3 poi = model_p->collider_main_point_absolute;
                //将下一步位置先一部求出，进行预判断,误差范围0.01f
                poi.y -= (rigidBody * _deletatime);
                vec3 lwh = model_p->collider_main_lwh;
                //还原当前模型的碰撞体
                Collider_Coboid* cco = new Collider_Coboid(poi, lwh);
                Collider_Coboid* ccco = new Collider_Coboid(vec3(poi.x, poi.y - 0.1f, poi.z), lwh);
                //获得离当前模型碰撞体最近的碰撞体
                vector<Collider_Coboid*> nccos = Collider_Layer_Default.collection_entity;//Collider_Layer_Default.Recent_Collision_Entity(poi, 50);
                //Print::Debug("重力检测");
                if (nccos.size() > 0)
                {
                    //Print::Debug("返回的碰撞体个数:" + to_string(nccos.size()));
                    bool _collision = false;
                    for (int i = 0; i < nccos.size(); i++)
                    {
                        if (Physics_Math_Down(cco, nccos[i], model_p, _deletatime) && Physics_Math_Down(ccco, nccos[i], model_p, _deletatime))
                        {
                            _collision = true;
                        }
                    }
                    if (!_collision)
                    {
                        //Print::Debug("false");
                        //position = *_position;
                        position.y -= rigidBody * _deletatime;
                    }
                    else
                    {
                        //Print::Debug("true");
                    }

                }
                //删除new出来的对象
                delete cco;
            }
        }
        //CheckCollider::Collision();
    }

    bool Physics_Math(Collider_Coboid* cco, Collider_Coboid* ncco, Model* model_p, float _deletatime, int xyz, vec3* _position, int guiwei)
    {
        vec3 poi = cco->Point();
        bool a = Collider_Coboid::Collision(cco, ncco);
        if (!a)
        {
            return false;
        }
        else
        {
            //if (guiwei == 0)
            //{
            //    float c_dif_x = cco->Point().x - ncco->Point().x;//点到点的距离
            //    float c_dif_y = cco->Point().y - ncco->Point().y;
            //    float c_dif_z = cco->Point().z - ncco->Point().z;
            //    float c_d_x_x = fabsf(c_dif_x) - (ncco->Length() + cco->Length());//点到点的距离的绝对值减去相应长宽高
            //    float c_d_y_x = fabsf(c_dif_y) - (ncco->Width() + cco->Width());
            //    float c_d_z_x = fabsf(c_dif_z) - (ncco->Height() + cco->Height());
            //    float final = Get_Min(c_d_x_x, c_d_y_x, c_d_z_x);//得到三个数中的最大的值
            //    if (false)
            //    {
            //        float differencey = model_p->position.y - model_p->collider_main_point_absolute.y;
            //        Print::Debug(to_string(c_dif_y));
            //        Print::Debug(to_string(c_d_y_x) + "X" + to_string(final));
            //        Print::Debug("XXXXX" + to_string(((ncco->Width() + cco->Width()) * 0.5f) + differencey));
            //        //position.y = ncco->Point().y - (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
            //        if (c_dif_y > 0)
            //        {
            //            poi.y = ncco->Point().y + (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
            //            //Print::Debug(to_string(ncco->Point().y) + "+((" + to_string(ncco->Width()) + "+" + to_string(cco->Width()) + ")*" + "0.5f" + ")+" + to_string(differencey));
            //        }
            //        else
            //        {
            //            poi.y = ncco->Point().y - (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
            //        }
            //        float differencex = model_p->position.x - model_p->collider_main_point_absolute.x;
            //        if (c_dif_x > 0)
            //        {
            //            poi.x = ncco->Point().x + (((ncco->Length() + cco->Length()) * 0.5f) + differencex);
            //        }
            //        else
            //        {
            //            poi.x = ncco->Point().x - (((ncco->Length() + cco->Length()) * 0.5f) + differencex);
            //        }
            //    }
            //    if (true)
            //    {
            //        //获得最近的坐标
            //        if (c_d_y_x == final && c_d_x_x - cco->Length() < 0 && c_d_z_x - cco->Height() < 0)
            //        {
            //            float differencey = model_p->position.y - model_p->collider_main_point_absolute.y;
            //            if (c_dif_y > 0)
            //            {
            //                position.y = ncco->Point().y + (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
            //                //Print::Debug(to_string(ncco->Point().y) + "+((" + to_string(ncco->Width()) + "+" + to_string(cco->Width()) + ")*" + "0.5f" + ")+" + to_string(differencey));
            //            }
            //            else
            //            {
            //                position.y = ncco->Point().y - (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
            //            }
            //            //Print::Debug(to_string(position.y) + "CXC" + to_string(c_d_y_x));
            //            //Print::Debug(to_string(cco->Point().x) + "X" + to_string(cco->Point().y) + "X" + to_string(cco->Point().z) + "X" + to_string(cco->Length()) + "X" + to_string(cco->Width()) + "X" + to_string(cco->Height()));
            //            //Print::Debug(to_string(ncco->Point().x) + "Y" + to_string(ncco->Point().y) + "Y" + to_string(ncco->Point().z) + "X" + to_string(ncco->Length()) + "X" + to_string(ncco->Width()) + "X" + to_string(ncco->Height()));
            //        }
            //        else if (c_d_z_x == final && false)
            //        {
            //            float differencez = model_p->position.z - model_p->collider_main_point_absolute.z;
            //            if (c_dif_z > 0)
            //            {
            //                position.z = ncco->Point().z + (((ncco->Width() + cco->Width()) * 0.5f) + differencez);
            //            }
            //            else
            //            {
            //                position.z = ncco->Point().z - (((ncco->Width() + cco->Width()) * 0.5f) + differencez);
            //            }
            //        }
            //        else if(c_d_y_x - cco->Width() < 0 && c_d_z_x - cco->Height() < 0)
            //        {
            //            float differencex = model_p->position.x - model_p->collider_main_point_absolute.x;
            //            if (c_dif_x > 0)
            //            {
            //                position.x = ncco->Point().x + (((ncco->Length() + cco->Length()) * 0.5f) + differencex);
            //            }
            //            else
            //            {
            //                position.x = ncco->Point().x - (((ncco->Length() + cco->Length()) * 0.5f) + differencex);
            //            }
            //        }
            //    }
            //    //cco->Point(poi);
            //}
        }
        return true;
    }

    bool Physics_Math_Down(Collider_Coboid* cco, Collider_Coboid* ncco, Model* model_p, float _deletatime)
    {
        bool a = Collider_Coboid::Collision(cco, ncco);
        if (!a)
        {
            return false;
        }
        else
        {
            float c_dif_y = cco->Point().y - ncco->Point().y;  //点到点的距离 
            float c_d_y_x = fabsf(c_dif_y) - (ncco->Width() + cco->Width());//点到点的距离的绝对值减去相应长宽高
            float c_dif_x = cco->Point().x - ncco->Point().x;
            float c_d_x_x = fabsf(c_dif_x) - (ncco->Length());
            float c_dif_z = cco->Point().z - ncco->Point().z;
            float c_d_z_x = fabsf(c_dif_z) - (ncco->Height());
            if (c_d_x_x < 0 && c_d_z_x < 0)
            {
                float differencey = model_p->position.y - model_p->collider_main_point_absolute.y;
                if (c_dif_y > 0)
                {
                    //position.y = ncco->Point().y + (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
                    //Print::Debug(to_string(ncco->Point().y) + "+((" + to_string(ncco->Width()) + "+" + to_string(cco->Width()) + ")*" + "0.5f" + ")+" + to_string(differencey));
                }
                else
                {
                    //position.y = ncco->Point().y - (((ncco->Width() + cco->Width()) * 0.5f) + differencey);
                    return true;
                }
            }
        }
        return true;
    }

    //得到所有数中最小的数
    float Get_Min(float _x, float _y, float _z)
    {
        if (_x < _y && _x < _z)
        {
            return _x;
        }
        else if (_y < _x && _y < _z)
        {
            return _y;
        }
        else
        {
            return _z;
        }
    }

    //得到所有数中绝对值最大的数
    float Get_Max_abs(float _x, float _y, float _z)
    {
        float a_x = fabsf(_x);
        float a_y = fabsf(_y);
        float a_z = fabsf(_z);
        if (a_x > a_y && a_x > a_z)
        {
            return _x;
        }
        else if (a_y > a_x && a_y > a_z)
        {
            return _y;
        }
        else
        {
            return _z;
        }
    }

    public:
        float RigidBody()
        {
            return rigidBody;
        }

        void RigidBody(float _rigiBody)
        {
            rigidBody = _rigiBody;
        }
};