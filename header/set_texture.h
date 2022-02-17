#pragma once
static GLuint Load_texture_image(string path);
//------
//环境纹理对象初始化（纹理对象，图片路径）
//------
GLuint Load_texture_environment(string path);
//加载纹理图像到显卡内存(图像数据，宽度，高度，加载模式)
static GLuint Load_texture_image(unsigned char* _image,int len, int _width, int _height, int _format);


//------
//带透明通道纹理对象初始化（纹理对象1，图片路径1）
//------
static void Load_texture(GLuint* texture, string path)
{
    *texture = Load_texture_image(path);
}

//------
//带透明通道纹理对象初始化（纹理对象1，纹理对象2，图片路径1，图片路径2）
//------
static void Load_texture(GLuint* texture1, GLuint* texture2, string path1, string path2)
{
    *texture1 = Load_texture_image(path1);
    *texture2 = Load_texture_image(path2);
}

//------
//带透明通道纹理对象初始化（纹理对象1，纹理对象2，纹理对象3，图片路径1，图片路径2，天空盒子路径3）
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
//带透明通道纹理对象初始化（纹理对象1，纹理对象2，纹理对象3， 纹理对象4，图片路径1，图片路径2，天空盒子路径3，法向量图片路径4）
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
//带透明通道纹理对象初始化（纹理对象，图片路径）
//------
static GLuint Load_texture_image(string path)
{
    GLuint texture;
    //图片的宽度、高度、读取到的图像类型
    int widthCout, height, nrComponents;
    //翻转y轴
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    //获取图片数据
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
    Print::Line("图像加载模式：" + to_string(nrComponents) + "::" + to_string(t) + path);
    if (image)
    {
        //生成纹理对象,根据纹理参数返回n个纹理索引//生成纹理ID
        glGenTextures(1, &texture);
        //绑定纹理对象texture1（绑定模式为2D）//绑定纹理对象到相应的索引//创建纹理对象
        glBindTexture(GL_TEXTURE_2D, texture);
        //target ：指定纹理单元的类型是哪一种，必须指定为 GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP中的一个。二维纹理需要指定为GL_TEXTURE_2D
        //
        //level：指定纹理单元的层次，非mipmap纹理level设置为0，mipmap纹理设置为纹理的层级(Mipmap是一种电脑图形图像技术，用于在三维图像的二维代替物中达到立体感效应。)
        //
        //internalFormat：指定OpenGL是如何管理纹理单元中数据格式的。网络上很多解释说这个参数必须和后面的format参数一样，这个说法是不正确的
        //
        //width：指定纹理单元的宽度
        //
        //height：指定纹理单元的高度
        //
        //border：指定纹理单元的边框，如果包含边框取值为1，不包含边框取值为0
        //
        //format：指定data所指向的数据的格式
        //
        //type：指定data所指向的数据的类型
        //
        //data：实际指向的数据是什么，这个参数有两种解释
        glTexImage2D(GL_TEXTURE_2D, 0, format, widthCout, height, 0, format, GL_UNSIGNED_BYTE, image);//指定一个二维纹理图像//指定纹理内容
        //生成Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        //绑定纹理//激活纹理功能
        //glBindTexture(GL_TEXTURE_2D, *texture); //(纹理要绑定到的目标,纹理名)// Unbind texture1 when done, so we won't accidentily mess up our texture1.
        //指定纹理属性
        //------------
        //设置纹理包裹参数(set the texture1 wrapping parameters)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //设置纹理过滤参数(set texture1 filtering parameters)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        Print::Line("Texture_alpha failed to load at path: " + path);
    }

    //提供纹理坐标
    //glTexCoord * ();
    stbi_image_free(image);
    return texture;
}

static GLuint Load_texture_image(unsigned char* _image, int len, int _width, int _height, int _format)
{
    GLuint texture;

    int nrComponents = 0;
    //翻转y轴
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

    //生成纹理对象,根据纹理参数返回n个纹理索引//生成纹理ID
    glGenTextures(1, &texture);
    //绑定纹理对象texture1（绑定模式为2D）//绑定纹理对象到相应的索引//创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture);
    //target ：指定纹理单元的类型是哪一种，必须指定为 GL_TEXTURE_2D, GL_PROXY_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, or GL_PROXY_TEXTURE_CUBE_MAP中的一个。二维纹理需要指定为GL_TEXTURE_2D
    //
    //level：指定纹理单元的层次，非mipmap纹理level设置为0，mipmap纹理设置为纹理的层级(Mipmap是一种电脑图形图像技术，用于在三维图像的二维代替物中达到立体感效应。)
    //
    //internalFormat：指定OpenGL是如何管理纹理单元中数据格式的。网络上很多解释说这个参数必须和后面的format参数一样，这个说法是不正确的
    //
    //width：指定纹理单元的宽度
    //
    //height：指定纹理单元的高度
    //
    //border：指定纹理单元的边框，如果包含边框取值为1，不包含边框取值为0
    //
    //format：指定data所指向的数据的格式
    //
    //type：指定data所指向的数据的类型
    //
    //data：实际指向的数据是什么，这个参数有两种解释
    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, image);//指定一个二维纹理图像//指定纹理内容
    //生成Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    //绑定纹理//激活纹理功能
    //glBindTexture(GL_TEXTURE_2D, *texture); //(纹理要绑定到的目标,纹理名)// Unbind texture1 when done, so we won't accidentily mess up our texture1.
    //指定纹理属性
    //------------
    //设置纹理包裹参数(set the texture1 wrapping parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //设置纹理过滤参数(set texture1 filtering parameters)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return texture;
}

static GLuint Load_texture_environment(string path) {
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    //加载路径
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
            Print::Line("立方体贴图加载失败/Cubemap texture failed to load at path: " + path);
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