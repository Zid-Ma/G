#pragma once
//加载顶点数组对象
void Load_vertices_object(GLuint* VAO, GLuint* VBO);
//加载顶点数组位置属性
void Load_vertices_attrib(float vertices[], int lengthCout);

//加载顶点数组对象和顶点缓冲对象
inline void Load_vertices(GLuint *VAO, GLuint *VBO, float vertices[], int lengthCout)
{
    Load_vertices_object(VAO, VBO);
    Load_vertices_attrib(vertices, lengthCout);
}

void Load_vertices_object(GLuint* VAO, GLuint* VBO)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    //绑定顶点数组对象
    glBindVertexArray(*VAO);
    //绑定顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
}

//加载顶点数组位置属性
void Load_vertices_attrib(float vertices[], int lengthCout)
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