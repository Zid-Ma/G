#pragma once

class Quad
{
private:
    static bool first;
protected:
    static GLuint vao, vbo;
public:
    Quad() 
    {
        First();
    }

    static void First()
    {
        if (first)
        {
            CreateVAO();
            first = false;
        }
    }

    static void CreateVAO()
    {
        // Ϊ��Ļ�ı��δ���������һ��VAO
        // screen quad VAO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), &Vertices_Quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        first = false;
    }

    static void SetVAO()
    {
        First();
        glBindVertexArray(vao);
    }
};
bool Quad::first = true;
GLuint Quad::vao = 0, Quad::vbo = 0;

class Bloom:Quad
{
private:
    static bool first;
public:
    static Shader shaderBlur, shaderFinal;
    static GLuint fbo, cbo[2], rbo;
    static GLuint pfbo[3], pcbo[3];
public:
    Bloom()
    {
        First();
    }

    static void First()
    {
        if (first)
        {
            Quad::First();
            Create();
            first = false;
        }
    }

    static void Create()
    {
        SetShader();

        //����Ҫ����һ��֡������󣬲�����
        //----------------------------------
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        //��ʹ�÷������ʱ
        // ---------------
        // ����2��������ɫ��������1������������Ⱦ����������������ֵ��
        //ͨ��ʹ��GL_COLOR_ATTACHMENT1�����ǿ��Եõ�һ��������������ɫ�����֡�������
        glGenTextures(2, cbo);
        //������ɫ�������
        for (GLuint i = 0; i < 2; i++)
        {
            if (false)//Multisampling > 0 && i == 0
            {
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, cbo[i]);
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Multisampling, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, cbo[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//����ʹ��FLOAT�������븡�㻺����
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if (false)//Multisampling > 0 && i == 0
            {
                //�������ӵ�֡������
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, cbo[i], 0);
            }
            else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, cbo[i], 0);
            }
        }
        if (Active_ssao)
        {
            glGenTextures(1, &gPositionDepth_SSAO);
            glBindTexture(GL_TEXTURE_2D, gPositionDepth_SSAO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gPositionDepth_SSAO, 0);
            // - Normal color buffer
            glGenTextures(1, &gNormal_SSAO);
            glBindTexture(GL_TEXTURE_2D, gNormal_SSAO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gNormal_SSAO, 0);
            // - Albedo color buffer
            glGenTextures(1, &gAlbedo_SSAO);
            glBindTexture(GL_TEXTURE_2D, gAlbedo_SSAO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gAlbedo_SSAO, 0);
        }
        unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4 };
        //��һ����ȾΪ�����������ڶ�����ȾΪ����������(���з������)
        //glDrawBuffers(5, attachments);
        //������������Ȼ�������renderbuffer��
        //-------------------------------------
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
        //�������ز�����Ȼ���
        //glRenderbufferStorageMultisample(GL_RENDERBUFFER, Multisampling, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        //����Ȼ�����󸽼ӵ�֡�������Ⱥ�ģ�帽����
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        // ����OpenGL���ǽ�ʹ�ã���֡�������ģ���Щ��ɫ����������Ⱦ//��Ҫ��ʽ��֪OpenGL��������ͨ��glDrawBuffers��Ⱦ�������ɫ���壬����OpenGLֻ����Ⱦ��֡����ĵ�һ����ɫ���������������������ġ����ǿ���ͨ�����ݶ����ɫ������ö�����������
        //unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4 };
        //��һ����ȾΪ�����������ڶ�����ȾΪ����������(���з������)
        glDrawBuffers(5, attachments);
        //���֡�����Ƿ�������
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print::Line("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        //���ǰ��֡����
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //ͼ���ģ������
        //----------------
        //Ϊͼ���ģ������������������֡���壬ÿ��ֻ��һ����ɫ��������
        glGenFramebuffers(3, pfbo);
        glGenTextures(3, pcbo);
        for (GLuint i = 0; i < 3; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pfbo[i]);
            if (i == 3)
            {
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, pcbo[i]);
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Multisampling, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);

            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, pcbo[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //�������ӵ�֡������
            if (i == 3)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, pcbo[i], 0);
            }
            else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pcbo[i], 0);
            }

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                Print::Line("Framebuffer not complete!");
        }
        //�ǵ�Ҫ���֡���壬��֤���ǲ��᲻С����Ⱦ�������֡������
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        first = false;
    }

    static void SetShader()
    {
        //��˹ģ����ɫ��
        shaderBlur = Shader(Path_Shader + "framebuffers.vert", Path_Shader + "framebuffers_bloom.frag");
        shaderFinal = Shader(Path_Shader + "framebuffers.vert", Path_Shader + "framebuffers_final.frag");//, Path_Shader + "framebuffers_final.geom"

        shaderBlur.use();
        shaderBlur.setInt("image", 0);

        //������ɫ����Ӧ����ͼ���id
        //ԭhdrͼ��
        shaderFinal.use();
        shaderFinal.setInt("screenTexture", 0);
        //����ģ��ͼ��
        shaderFinal.setInt("bloomBlur", 1);
        shaderFinal.setBool("bloom", true);
        shaderFinal.setFloat("exposure", 1.0f);//�ع�ֵ
    }
};
bool Bloom::first = true;
Shader Bloom::shaderBlur, Bloom::shaderFinal;
GLuint Bloom::fbo = 0, Bloom::cbo[2] = { 0,0 }, Bloom::rbo = 0;
GLuint Bloom::pfbo[3] = { 0,0,0 }, Bloom::pcbo[3] = { 0,0,0 };

class FBO:Quad
{
private:
    GLuint fBO, cBO, rBO;
public:

    //��ȡһ��֡�������(֡���壬��ɫ���壬��Ⱦ����)
    void Create()//GLuint* fbo, GLuint* cbo, GLuint* rbo
    {
        GLuint Quad_FBO, Quad_CBO, Quad_RBO;
        glGenFramebuffers(1, &Quad_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, Quad_FBO);
        glGenTextures(1, &Quad_CBO);
        glBindTexture(GL_TEXTURE_2D, Quad_CBO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Quad_CBO, 0);
        glGenRenderbuffers(1, &Bloom::rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, Bloom::rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Bloom::rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print::Line("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Get(GLuint* fbo, GLuint* cbo, GLuint* rbo)
    {
        *fbo = fBO;
        *cbo = cBO;
        *rbo = rBO;
    }
};

static class UI :Quad
{
private:
    static GLboolean first;
    static Shader shader;
    static GLuint vAO, vBO;
    static GLuint texture;
private:
    static Shader fontShader;
public:
    UI()
    {
        First();
        shader = Shader(Path_Shader + "ui.vert", Path_Shader + "ui.frag");
        fontShader = Shader(Path_Shader + "ui.vert", Path_Shader + "ui_font.frag");
    }

    UI(string path_vert, string path_frag)
    {
        shader = Shader(path_vert,path_frag);
    }

    ~UI()
    {

    }

    void First()
    {
        if (first)
        {
            Create();
            //Load_texture(&texture, Path_Text + "text.png");
            Load_texture(&texture, Path_Text + TextSystem::MapPath());
            Print::Debug("UI��������ʼ����" + to_string(texture));
            first = false;
        }
    }

    void Use()
    {
        First();
        shader.use();
        //Load_texture(&texture, Path_Text + TextSystem::MapPath());
        glBindVertexArray(vAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
    }

    void Use_shader()
    {
        shader.use();
    }

    void Use_fontShader()
    {
        fontShader.use();
        //FontColor(vec4(0.2f,0.4f,0.4f,1.0f));
        FontColor(vec4(0.0f));
    }
    Shader FontShader()
    {
        return fontShader;
    }

    static void Create()
    {
        char c = 'X';
        int w = TextSystem::LengthCount(c);
        int l = TextSystem::WidthCount(c);
        Print::Debug("UI���󴴽�ʱ��λ��(w��l)��" + to_string(w) + "www" + to_string(l));
        //��Ⱦui�ı�������
        float scr_min = SCR_WIDTH;
        float scr_max = SCR_HEIGHT;
        float scr_w = scr_min / scr_min;
        float scr_h = scr_min / scr_max;
        //��Ⱦui��λ������
        float ui_length = 50.0f / scr_min;//ui��С
        float ui_width = 50.0f / scr_min;
        float l_cout = 300.0f / SCR_HEIGHT;//��������Ӧ��λ��ƫ��
        float w_cout = -500.0f / SCR_WIDTH;
        float vertices_Quad[] = {
            // positions   // texCoords
            (-1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  l * TextSystem::Length(), (w + 1) * TextSystem::Width(),//����
            (-1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  l* TextSystem::Length(), w * TextSystem::Width(),//����
             (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  (l+1)* TextSystem::Length(), w* TextSystem::Width(),//����

            (-1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  l* TextSystem::Length(), (w + 1)* TextSystem::Width(),//����
             (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  (l + 1)* TextSystem::Length(), w* TextSystem::Width(),//����
             (1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  (l + 1)* TextSystem::Length(), (w + 1)* TextSystem::Width()//����
        };
        // Ϊ��Ļ�ı��δ���������һ��VAO
        // screen quad VAO
        glGenVertexArrays(1, &vAO);
        glGenBuffers(1, &vBO);
        glBindVertexArray(vAO);
        glBindBuffer(GL_ARRAY_BUFFER, vBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Quad), &vertices_Quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    /// <summary>
    /// ����Ļ�л����ı�
    /// </summary>
    /// <param name="_s">�ı�����</param>
    /// <param name="_widthPoint">��Ļ�ռ���������</param>
    /// <param name="_lengthPoint">��Ļ�ռ����������</param>
    /// <param name="_pixels">�ı���С</param>
    static void DrawTexts(string _s, float _widthPoint, float _lengthPoint, float _pixels)
    {
        //��Ⱦui�ı�������
        float scr_min = SCR_WIDTH;
        float scr_max = SCR_HEIGHT;
        float scr_w = scr_min / scr_min;
        float scr_h = scr_min / scr_max;
        //��Ⱦui��λ������
        float ui_length = _pixels / (scr_min * 1);//ui��С
        float ui_width = _pixels / (scr_min * 1);
        float l_cout = _lengthPoint * 2 / SCR_HEIGHT;//��������Ӧ��λ��ƫ��
        float w_cout = _widthPoint * 2 / SCR_WIDTH;
        for (int i = 0; i < _s.length(); i++)
        {
            const char c = _s.substr(i, 1).c_str()[0];
            int w = TextSystem::LengthCount(c);
            int l = TextSystem::WidthCount(c);
            w_cout += ui_width * 2;
            float vertices_Quad[] = {
                // positions   // texCoords
                (-1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  l * TextSystem::Length(), (w + 1) * TextSystem::Width(),//����
                (-1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  l * TextSystem::Length(), w * TextSystem::Width(),//����
                 (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  (l + 1) * TextSystem::Length(), w * TextSystem::Width(),//����

                (-1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  l * TextSystem::Length(), (w + 1) * TextSystem::Width(),//����
                 (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  (l + 1) * TextSystem::Length(), w * TextSystem::Width(),//����
                 (1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  (l + 1) * TextSystem::Length(), (w + 1) * TextSystem::Width()//����
            };
            glBindVertexArray(vAO);
            glBindBuffer(GL_ARRAY_BUFFER, vBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Quad), &vertices_Quad, GL_STATIC_DRAW);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            //Print::Debug(to_string(texture));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void DrawUI(string _s, float _widthPoint, float _lengthPoint, float _size)
    {

    }

    void DrawImage(GLuint _texture, float _widthPoint, float _lengthPoint, float _width, float _height)
    {
        float scr_min = SCR_WIDTH;
        float scr_max = SCR_HEIGHT;
        float scr_w = scr_min / scr_min;//��߱�
        float scr_h = scr_min / scr_max;
        float ui_length = _height / (scr_min);//ui��С
        float ui_width = _width / (scr_min);
        float l_cout = _lengthPoint * 2 / SCR_HEIGHT;//��������Ӧ��λ��ƫ��
        float w_cout = _widthPoint * 2 / SCR_WIDTH;
        float vertices_Quad[] = {
            (-1.0f * ui_width * scr_w) + w_cout, (1.0f * ui_length * scr_h) + l_cout, 0.0f, 1.0f,
            (-1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  0.0f, 0.0f,
            (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  1.0f, 0.0f,

            (-1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  0.0f, 1.0f,
            (1.0f * ui_width * scr_w) + w_cout, (-1.0f * ui_length * scr_h) + l_cout,  1.0f, 0.0f,
            (1.0f * ui_width * scr_w) + w_cout,  (1.0f * ui_length * scr_h) + l_cout,  1.0f, 1.0f
        };
        glBindVertexArray(vAO);
        glBindBuffer(GL_ARRAY_BUFFER, vBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_Quad), &vertices_Quad, GL_STATIC_DRAW);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void FontColor(vec4 _rgba)
    {
        fontShader.setVec4("color", _rgba);
    }
};
GLboolean UI::first = true;
Shader UI::shader;
GLuint UI::vAO = 0, UI::vBO = 0;
GLuint UI::texture = 0;
Shader UI::fontShader;