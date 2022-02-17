#pragma once
static GLuint Load_texture_image(string path);
//------
//������������ʼ�����������ͼƬ·����
//------
GLuint Load_texture_environment(string path);
//��������ͼ���Կ��ڴ�(ͼ�����ݣ���ȣ��߶ȣ�����ģʽ)
static GLuint Load_texture_image(unsigned char* _image,int len, int _width, int _height, int _format);


//------
//��͸��ͨ����������ʼ�����������1��ͼƬ·��1��
//------
static void Load_texture(GLuint* texture, string path)
{
    *texture = Load_texture_image(path);
}

//------
//��͸��ͨ����������ʼ�����������1���������2��ͼƬ·��1��ͼƬ·��2��
//------
static void Load_texture(GLuint* texture1, GLuint* texture2, string path1, string path2)
{
    *texture1 = Load_texture_image(path1);
    *texture2 = Load_texture_image(path2);
}

//------
//��͸��ͨ����������ʼ�����������1���������2���������3��ͼƬ·��1��ͼƬ·��2����պ���·��3��
//------
static void Load_texture(GLuint* texture1, GLuint* texture2, GLuint* texture3, string path1, string path2, string path3) {
    if (path1 != "")
    {
        *texture1 = Load_texture_image(path1);
        cout << texture1 << "x+x" << path1 << endl;
    }
    if (path2 != "")
    {
        *texture2 = Load_texture_image(path2);
        cout << texture2 << "y+y" << path2 << endl;
    }
    if(path3 != "")
    *texture3 = Load_texture_environment(path3);
}

//------
//��͸��ͨ����������ʼ�����������1���������2���������3�� �������4��ͼƬ·��1��ͼƬ·��2����պ���·��3��������ͼƬ·��4��
//------
static void Load_texture(GLuint* texture1, GLuint* texture2, GLuint* texture3, GLuint* texture4, string path1, string path2, string path3, string path4) {
    if (path1 != "")
    {
        *texture1 = Load_texture_image(path1);
        cout << texture1 << "x+x" << path1 << endl;
    }
    if (path2 != "")
    {
        *texture2 = Load_texture_image(path2);
        cout << texture2 << "y+y" << path2 << endl;
    }
    if (path3 != "")
    *texture3 = Load_texture_environment(path3);
    if (path4 != "")
    *texture4 = Load_texture_image(path4);
}

//------
//��͸��ͨ����������ʼ�����������ͼƬ·����
//------
static GLuint Load_texture_image(string path)
{
    GLuint texture;
    //ͼƬ�Ŀ�ȡ��߶ȡ���ȡ����ͼ������
    int widthCout, height, nrComponents;
    //��תy��
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //��ȡͼƬ����
    unsigned char* image = stbi_load(path.c_str(), &widthCout, &height, &nrComponents, 0);
    GLenum format = 3;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;
    //Print::Debug(to_string(nrComponents));

    int t = (int)&texture;
    //cout << t << path << endl;
    Print::Line("ͼ�����ģʽ��" + to_string(nrComponents) + "::" + to_string(t) + path);
    if (image)
    {
        //�����������,���������������n����������//��������ID
        glGenTextures(1, &texture);
        //���������texture1����ģʽΪ2D��//�����������Ӧ������//�����������
        glBindTexture(GL_TEXTURE_2D, texture);
        //target ��ָ������Ԫ����������һ�֣�����ָ��Ϊ GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP�е�һ������ά������Ҫָ��ΪGL_TEXTURE_2D
        //
        //level��ָ������Ԫ�Ĳ�Σ���mipmap����level����Ϊ0��mipmap��������Ϊ����Ĳ㼶(Mipmap��һ�ֵ���ͼ��ͼ��������������άͼ��Ķ�ά�������дﵽ�����ЧӦ��)
        //
        //internalFormat��ָ��OpenGL����ι�������Ԫ�����ݸ�ʽ�ġ������Ϻܶ����˵�����������ͺ����format����һ�������˵���ǲ���ȷ��
        //
        //width��ָ������Ԫ�Ŀ��
        //
        //height��ָ������Ԫ�ĸ߶�
        //
        //border��ָ������Ԫ�ı߿���������߿�ȡֵΪ1���������߿�ȡֵΪ0
        //
        //format��ָ��data��ָ������ݵĸ�ʽ
        //
        //type��ָ��data��ָ������ݵ�����
        //
        //data��ʵ��ָ���������ʲô��������������ֽ���
        glTexImage2D(GL_TEXTURE_2D, 0, format, widthCout, height, 0, format, GL_UNSIGNED_BYTE, image);//ָ��һ����ά����ͼ��//ָ����������
        //����Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        //������//����������
        //glBindTexture(GL_TEXTURE_2D, *texture); //(����Ҫ�󶨵���Ŀ��,������)// Unbind texture1 when done, so we won't accidentily mess up our texture1.
        //ָ����������
        //------------
        //���������������(set the texture1 wrapping parameters)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //����������˲���(set texture1 filtering parameters)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        Print::Line("Texture_alpha failed to load at path: " + path);
    }

    //�ṩ��������
    //glTexCoord * ();
    stbi_image_free(image);
    return texture;
}

static GLuint Load_texture_image(unsigned char* _image, int len, int _width, int _height, int _format)
{
    GLuint texture;

    int nrComponents = 0;
    //��תy��
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load_from_memory(_image, len, &_width, &_height, &nrComponents, 4);
    //if (nrComponents == 1)
    //    _format = GL_RED;
    //else if (nrComponents == 3)
    //    _format = GL_RGB;
    //else if (nrComponents == 4)
    //    _format = GL_RGBA;
    //else
    //    _format = GL_RGB;
    //Print::Debug(to_string(nrComponents));

    //�����������,���������������n����������//��������ID
    glGenTextures(1, &texture);
    //���������texture1����ģʽΪ2D��//�����������Ӧ������//�����������
    glBindTexture(GL_TEXTURE_2D, texture);
    //target ��ָ������Ԫ����������һ�֣�����ָ��Ϊ GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP�е�һ������ά������Ҫָ��ΪGL_TEXTURE_2D
    //
    //level��ָ������Ԫ�Ĳ�Σ���mipmap����level����Ϊ0��mipmap��������Ϊ����Ĳ㼶(Mipmap��һ�ֵ���ͼ��ͼ��������������άͼ��Ķ�ά�������дﵽ�����ЧӦ��)
    //
    //internalFormat��ָ��OpenGL����ι�������Ԫ�����ݸ�ʽ�ġ������Ϻܶ����˵�����������ͺ����format����һ�������˵���ǲ���ȷ��
    //
    //width��ָ������Ԫ�Ŀ��
    //
    //height��ָ������Ԫ�ĸ߶�
    //
    //border��ָ������Ԫ�ı߿���������߿�ȡֵΪ1���������߿�ȡֵΪ0
    //
    //format��ָ��data��ָ������ݵĸ�ʽ
    //
    //type��ָ��data��ָ������ݵ�����
    //
    //data��ʵ��ָ���������ʲô��������������ֽ���
    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, image);//ָ��һ����ά����ͼ��//ָ����������
    //����Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //������//����������
    //glBindTexture(GL_TEXTURE_2D, *texture); //(����Ҫ�󶨵���Ŀ��,������)// Unbind texture1 when done, so we won't accidentily mess up our texture1.
    //ָ����������
    //------------
    //���������������(set the texture1 wrapping parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //����������˲���(set texture1 filtering parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return texture;
}

static GLuint Load_texture_environment(string path) {
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    //����·��
    vector<std::string> faces
    {
        path + "right.jpg",
        path + "left.jpg",
        path + "top.jpg",
        path + "bottom.jpg",
        path + "front.jpg",
        path + "back.jpg"
    };

    int widthCout, height, nrComponents;
    for (GLuint i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &widthCout, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthCout, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            Print::Line("��������ͼ����ʧ��/Cubemap texture failed to load at path: " + path);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}