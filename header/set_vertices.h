#pragma once
//���ض����������
void Load_vertices_object(GLuint* VAO, GLuint* VBO);
//���ض�������λ������
void Load_vertices_attrib(float vertices[], int lengthCout);

//���ض����������Ͷ��㻺�����
inline void Load_vertices(GLuint *VAO, GLuint *VBO, float vertices[], int lengthCout)
{
    Load_vertices_object(VAO, VBO);
    Load_vertices_attrib(vertices, lengthCout);
}

void Load_vertices_object(GLuint* VAO, GLuint* VBO)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    //�󶨶����������
    glBindVertexArray(*VAO);
    //�󶨶��㻺�����
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
}

//���ض�������λ������
void Load_vertices_attrib(float vertices[], int lengthCout)
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