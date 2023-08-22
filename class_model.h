#pragma once
//��UTF8�����ʽת��ΪGBT32
//string UTF8ToGB(const char* str);
//��string����ת��Ϊfloat����
//float StringTofloat(string s);

class Model
{
private:
    string model_type[5] = { "", "", "", "", "" };
    string texture_path[5] = { "", "", "", "", "" };//����·��
    int vertices_length = 0;
    GLfloat* vertices;//ģ������
private:
    bool first = true;
    string XTpath = "";
    string Xpath = "";
    GLuint vao = 0, vbo = 0;
    GLuint texture0 = 0, texture1 = 0, texture2 = 0, texture3 = 0;
    Shader shader;
public:
    //�ж��Ƿ�Ϊ������ģ�Ͷ���
    bool alone = true;
    //��ǰģ�͵�λ��
    vec3 position;
public:
    bool rigid_active;
    float rigidBody;
    string collider_main_type;//��ײ������
    string collider_attack_type;//��ײ������
    vec3 collider_main_lwh;
    float collider_attack_radius;
    bool collider_main_active = false, collider_attack_active = false;//�Ƿ�������ײ��
    vec3 collider_main_point, collider_attack_point;//���λ��
    vec3 collider_main_point_absolute, collider_attack_point_absolute;//����λ��
public:
    Model()
    {
        first = true;
        Print::Debug("������һ��ģ����...");
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
            Print::Debug("���ڳ���ɾ����̬������ģ������...");
            if (vertices != NULL)
                delete[] vertices;
        }
        catch (exception e)
        {
            Print::Debug("ɾ����̬������ģ������ʱ���ִ���");
            Print::Debug(e.what());
        }
    }

    string Get_name()
    {
        return texture_path[0];
    }

    void Get_Xpath(string _path)
    {
        //��ȡ�����һ��б�ܵ�λ��
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
        Print::Debug("ModelX�ļ�λ�ã�" + Xpath);
    }

    void First()
    {
        if (first)
        {
            Print::Debug("��ǰģ��λ��:" + Xpath);
            Print::Debug("����ģ�Ͷ���...");
            load_vao(&vao, &vbo);
            Print::Debug(to_string(sizeof(vertices)));
            Print::Debug("����ģ����Ⱦ����...");
            load_vbo(vertices, sizeof(GLfloat) * vertices_length);
            shader = Shader(Path_Shader + "multiple.vert", Path_Shader + "multiple.frag");
            Load_shader(shader);
            Print::Debug("����ģ����ͼ...");
            string t1 = "", t2 = "", t3 = "", t4 ="";
            if (texture_path[0] != "")t1 = Xpath + texture_path[0];
            if (texture_path[1] != "")t2 = Xpath + texture_path[1];
            if (texture_path[2] != "")t3 = Xpath + texture_path[2];
            if (texture_path[3] != "")t4 = Xpath + texture_path[3];
            Load_texture(&texture0, &texture1, &texture2, &texture3, t1, t2, t3, t4);
            Print::Debug("ģ�ͳ�ʼ�����.");
            Print::Debug(XTpath);
            first = false;
        }
    }

    //��Ϊ����ģ��ʱ
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
                //������ײ�壬����ӵ���ʵ��ء���ȥ
                Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh);//��ȡ����λ�õ���ײ��
                CCo->type = collider_main_type;
                Collider_Layer_Default.collection_entity.push_back(CCo);
            }
            //�����������
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
    //�������(�������������ж�)
    static void Physics_Gravity(Model* model_p, float deletatime)
    {
        if (model_p->rigid_active)
        {
            model_p->rigid_active = true;
            model_p->rigidBody = model_p->rigidBody;
            //Print::Debug("�������");
            if (model_p->collider_main_active)
            {
                //��ȡ����ײ�������������ϵ��λ��
                vec3 poi = model_p->collider_main_point_absolute;
                //����һ��λ����һ�����������Ԥ�ж�,��Χ0.01f
                poi.y -= model_p->rigidBody * deletatime + 0.01f;
                vec3 lwh = model_p->collider_main_lwh;
                //��ԭ��ǰģ�͵���ײ��
                Collider_Coboid* cco = new Collider_Coboid(poi, lwh);
                //����뵱ǰģ����ײ���������ײ��
                Collider_Coboid* ncco = Collider_Layer_Default.Recent_Collision_Entity(poi);
                //Print::Debug("�������");
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
                        //������������
                        model_p->position.y = ncco->Point().y + ((ncco->Width() + cco->Width()) * 0.5f) + difference;
                        //Print::Debug(to_string(model_p->position.y));
                    }
                }
                //ɾ��new�����Ķ���
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
                //�õ���ײ�����λ��
                collider_main_point_absolute = collider_main_point + position;
            }
            //�����������
            if (rigidBody > 0.0f)
            {
                Physics_Gravity(this, DeltaTime);
            }
            if (collider_attack_active)
            {
                //�õ�������ײ�����λ��
                collider_attack_point_absolute = collider_attack_point + position;
            }
        }
    }
    //���������λ��ֵ���иı�,ֱ�ӻ���
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
                //�õ���ײ�����λ��
                vec3 cmpa = collider_main_point + _position;
                //������ײ�壬����ӵ���ʵ��ء���ȥ
                Collider_Coboid* CCo = new Collider_Coboid(cmpa, collider_main_lwh);//��ȡ����λ�õ���ײ��
                CCo->type = collider_main_type;
                Collider_Layer_Default.collection_entity.push_back(CCo);
            }
            //�����������
            if (rigidBody > 0.0f)
            {
                Physics_Gravity(this, DeltaTime);
            }
            if (collider_attack_active)
            {
                //�õ�������ײ�����λ��
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
        //        //������ײ�壬����ӵ���ʵ��ء���ȥ
        //        Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh);//��ȡ����λ�õ���ײ��
        //        CCo->type = collider_main_type;
        //        collection_entity.push_back(CCo);
        //    }
        //    //�����������
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

    //���ƶ��ģ��(λ�����飬����)
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
    //������޸�
    void Draw_Collider(vec3 _position, vec3 _scale)
    {
        if (first && alone)position = _position;
        First();
        //Print::Debug("???x");
        if (collider_main_active)
        {
            collider_main_point_absolute = collider_main_point + _position;
            //������ײ�壬����ӵ���ʵ��ء���ȥ
            Collider_Coboid* CCo = new Collider_Coboid(collider_main_point_absolute, collider_main_lwh * _scale);//��ȡ����λ�õ���ײ��
            CCo->type = collider_main_type;
            Collider_Layer_Default.collection_entity.push_back(CCo);
        }
        //Print::Debug("???y");
        //�����������
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
        //�����������
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
        //���ж�ȡ
        ifstream infile;
        infile.open(path.data());   //���ļ����������ļ��������� 
        assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

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
        infile.close();             //�ر��ļ������� 
    }

    //ʶ������
    bool Line(string s)
    {
        //�����һ��Ϊ#����������Ϊע��
        if (s.substr(0, 1) == "#")
        {

        }
        else if (s.length() > 4 + 1)
        {
            if (s.substr(0, 4) == "�汾")
            {
                model_type[0] = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "����")
            {
                model_type[1] = s.substr(4 + 1);
            }
            else if (s.substr(0, 4) == "��ʽ")
            {
                model_type[2] = s.substr(4 + 1);
            }
            else if (s.length() > 6 + 1)
            {
                if (s.substr(0, 6) == "��ײ��")
                {
                    if (s.substr(6 + 1, 8) == "���幥��")
                    {
                        string pengzhuangti = s.substr(6 + 1 + 8);
                        string radius_s = "";//��ײ����(�뾶)
                        int pointc = 0;//vec3����
                        vec3 point3;//�����
                        string point = "";
                        for (int i = 0; i < pengzhuangti.length(); i++)
                        {
                            string is = pengzhuangti.substr(i, 1);
                            if (point == "")
                            {
                                //δ����������ʱ
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
                    else if(s.substr(6 + 1, 8) == "��Ҫ����")
                    {
                        string pengzhuangti = s.substr(6 + 1 + 8);
                        string lwh_s = "";//��ײ�峤���
                        int lwhc = 0;
                        vec3 lwh;
                        int pointc = 0;//vec3����
                        vec3 point3;//�����
                        string point = "";
                        for (int i = 0; i < pengzhuangti.length(); i++)
                        {
                            string is = pengzhuangti.substr(i, 1);
                            if (point == "")
                            {
                                //δ����������ʱ
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
                    else if (s.substr(6 + 1, 8) == "��Ϊʵ��")
                    {
                        if (s.substr(6 + 1 + 8, 8) != "��Ϊʵ��")
                            rigidBody = stringTofloat(s.substr(6 + 1 + 8 + 1));//���������������֤����������
                        rigid_active = true;
                    }
                    else if (s.substr(6 + 1, 8) == "��ײ����")
                    {
                        if (s.substr(6 + 1 + 8 + 1, 8) != "���幥��")
                            collider_attack_type = s.substr(6 + 1 + 8 + 1 + 8 + 1);
                        if (s.substr(6 + 1 + 8 + 1, 8) != "��Ҫ����")
                            collider_main_type = s.substr(6 + 1 + 8 + 1 + 8 + 1);

                    }
                }
                else if (s.substr(0, 6) == "������") 
                {
                    vertices_length = 8 * atoi(s.substr(6 + 1).c_str());
                    //��̬����һ������
                    vertices = new GLfloat[vertices_length];
                    Print::Debug("��̬�����С:" + to_string(vertices_length));
                }
                else if (s.substr(0, 6) == "������")
                {
                    texture_path[0] = s.substr(6 + 1);
                    Print::Debug("������:" + texture_path[0]);
                }
                else if (s.length() > 8 + 1)
                {
                    if (s.substr(0, 8) == "���淴��")
                    {
                        texture_path[1] = s.substr(8 + 1);
                    }
                    else if (s.substr(0, 8) == "��������")
                    {
                        texture_path[2] = s.substr(8 + 1);
                        Print::Debug("������");
                        Print::Debug("��������:" + texture_path[2]);
                    }
                    else if (s.substr(0, 8) == "������ͼ")
                    {
                        texture_path[3] = s.substr(8 + 1);
                    }
                }
            }

        }
        else if (s.substr(0, 4) == "����")
        {
            Print::Debug(s.substr(0, 4) + "model" + s);
            return true;
        }
        return false;
    }

    int count = 0;
    //��ȡ����
    bool Data(string s) 
    {
        //��ȡ��ע��ʱ����
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

    //��ȡÿһ������ǰ��С���ݲ�����������
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

    //ģ�庯������string���ͱ���ת��Ϊ���õ���ֵ���ͣ��˷��������ձ������ԣ�  
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

        //�󶨶����������
        glBindVertexArray(*VAO);
        //�󶨶��㻺�����
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    }

    //���ض�������λ������(�������飬 ����)
    static void load_vbo(float vertices[], int lengthCout)
    {
        //��������ʼ����������������ݴ洢
        glBufferData(GL_ARRAY_BUFFER, lengthCout, vertices, GL_STATIC_DRAW);

        //�Զ��㻺��������ݽ��н�������
        // -----------------------------
        //λ������
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);//���ö�������0
        //����������
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //������������
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);//���ö�������1
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

