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
    //位置 position
    glm::vec3 Position;
    //归一化(法向量) normal
    glm::vec3 Normal;
    //纹理坐标 texCoords
    glm::vec2 TexCoords;
    //切线 tangent
    glm::vec3 Tangent;
    //双切线 bitangent
    glm::vec3 Bitangent;
};

//纹理
//----
struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    //网格数据 mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO;

    //构造器 constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        //现在我们有了所有必需的数据，设置顶点缓冲区及其属性指针。 now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    //渲染网格 render the mesh
    void Draw(Shader& shader)
    {
        //绑定适当的纹理 bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        unsigned int reflection = 1;
        //遍历整个纹理数组
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); //绑定前激活适当的纹理单元 active_shader proper texture unit before binding
            //检索纹理编号（漫反射纹理中的N） retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;//获取纹理类型（纹理名）
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); //传输无符号整数到流 transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream
            else if (name == "texture_reflection")
                number = std::to_string(reflection++);


            //现在将采样器设置为正确的纹理单位 now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            //最后绑定纹理 and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        //绘制网格 draw mesh
        glBindVertexArray(VAO);
        //mode:接受的值和在glBegin()中接受的值一样，可以是GL_POLYGON、GL_TRIANGLES、GL_TRIANGLE_STRIP、GL_LINE_STRIP等。
        //count：组合几何图形的元素的个数，一般是点的个数。
        //type : indeices数组的数据类型，既然是索引，一般是整型的。
        //indices : 索引数组
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //一旦配置，将所有内容都设置回默认值始终是一种良好的做法。 always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    //渲染数据 render data 
    unsigned int VBO, EBO;

    //安装所有的缓冲对象或数组 initializes all the buffer objects/arrays
    void setupMesh()
    {
        //创建缓冲区/阵列 create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        //加载数据到顶点缓冲中 load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //结构的一个优点是，它们的内存布局对其所有项都是顺序的。 A great thing about structs is that their memory layout is sequential for all its items.
        //其效果是，我们可以简单地将一个指针传递给结构，然后将其完美地转换为glm:：vec3/2数组，该数组 The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        //再次转换为3/2浮点，转换为字节数组。 again translates to 3/2 floats which translates to a byte array.
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