#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <shader_me.h>

#include <string>
#include <vector>
using namespace std;

struct Vertex {
    //λ�� position
    glm::vec3 Position;
    //��һ��(������) normal
    glm::vec3 Normal;
    //�������� texCoords
    glm::vec2 TexCoords;
    //���� tangent
    glm::vec3 Tangent;
    //˫���� bitangent
    glm::vec3 Bitangent;
};

//����
//----
struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    //�������� mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    //������ constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        //���������������б�������ݣ����ö��㻺������������ָ�롣 now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    //��Ⱦ���� render the mesh
    void Draw(Shader& shader)
    {
        //���ʵ������� bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        unsigned int reflection = 1;
        //����������������
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); //��ǰ�����ʵ�������Ԫ active_shader proper texture unit before binding
            //���������ţ������������е�N�� retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;//��ȡ�������ͣ���������
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); //�����޷����������� transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream
            else if (name == "texture_reflection")
                number = std::to_string(reflection++);


            //���ڽ�����������Ϊ��ȷ������λ now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            //�������� and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        //�������� draw mesh
        glBindVertexArray(VAO);
        //mode:���ܵ�ֵ����glBegin()�н��ܵ�ֵһ����������GL_POLYGON��GL_TRIANGLES��GL_TRIANGLE_STRIP��GL_LINE_STRIP�ȡ�
        //count����ϼ���ͼ�ε�Ԫ�صĸ�����һ���ǵ�ĸ�����
        //type : indeices������������ͣ���Ȼ��������һ�������͵ġ�
        //indices : ��������
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //һ�����ã����������ݶ����û�Ĭ��ֵʼ����һ�����õ������� always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    //��Ⱦ���� render data 
    unsigned int VBO, EBO;

    //��װ���еĻ����������� initializes all the buffer objects/arrays
    void setupMesh()
    {
        //����������/���� create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        //�������ݵ����㻺���� load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //�ṹ��һ���ŵ��ǣ����ǵ��ڴ沼�ֶ����������˳��ġ� A great thing about structs is that their memory layout is sequential for all its items.
        //��Ч���ǣ����ǿ��Լ򵥵ؽ�һ��ָ�봫�ݸ��ṹ��Ȼ����������ת��Ϊglm:��vec3/2���飬������ The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        //�ٴ�ת��Ϊ3/2���㣬ת��Ϊ�ֽ����顣 again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};
#endif