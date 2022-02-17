#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb-master/stb_image.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include <mesh_me.h>
//#include <shader_me.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    //ģ������
    // model data 
    //�洢����Ϊֹ���ص����������������Ż���ȷ����������ض�Ρ�
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    //���캯������Ҫָ����άģ�͵��ļ�·����
    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    //����ģ�ͼ�����������
    // draws the model, and thus all its meshes
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    //���ļ����ؾ���֧�ֵ�ASIMP��չ����ģ�ͣ��������ɵ�����洢�����������С�
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path)
    {
        //ͨ��ASSIMP��ȡ�ļ�
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //������
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        //�����ļ�·����Ŀ¼·��
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        //�ݹ鴦��ASSIMP�ĸ��ڵ�
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    //�Եݹ鷽ʽ����ڵ㡣����λ�ڽڵ��ϵ�ÿ���������񣬲������ӽڵ㣨����У����ظ��˹��̡�
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        //����λ�ڵ�ǰ�ڵ��ÿ������
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            //�ڵ������������������������е�ʵ�ʶ���
            //�������������е����ݣ��ڵ�ֻ��Ϊ��ʹ���ݱ������򣨱���ڵ�֮��Ĺ�ϵ����
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //������������������У������ǽ��ݹ鴦��ÿ���ӽڵ�
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        //���������ÿ������
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            //��������һ��ռλ����������Ϊassimpʹ�����Լ��������࣬������ֱ��ת��Ϊglm��vec3�࣬�����������Ƚ����ݴ��䵽��ռλ��glm:��vec3��
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            //λ��
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            //��һ��
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            //��������
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?//�����Ƿ�����������ꣿ
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                ////һ�����������԰���8����ͬ���������ꡣ��ˣ����Ǽ������ǲ���
                //ʹ��һ����������ж�����������ģ�ͣ��������ʼ��ʹ�õ�һ�飨0����
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent//���У����ߣ�
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent//˫����
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        //���ڱ��������ÿ���棨���������������Σ���������Ӧ�Ķ���������
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            //���������������������洢������������
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        //�ӹ�����
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        //���Ǽ�����ɫ���еĲ�����������һ��Լ����Ӧ����ÿ������������
        //��Ϊ��������ɢ��������N�Ǵ�1������������ŵ����кš�
        //�����б��ܽ������ͬ����������������
        //�����䣺����\������
        //���淴�䣺����\���淴��
        //���ߣ�����\����

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_diffuse");//aiTextureType_AMBIENT//aiTextureType_DIFFUSE
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");//aiTextureType_SPECULAR
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");//aiTextureType_HEIGHT,Assimp��aiTextureType_NORMAL������������ķ�����ͼ����aiTextureType_HEIGHTȴ��
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        //  5.reflection maps//����ӳ��
        std::vector<Texture> reflectionTexture = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
        textures.insert(textures.end(), reflectionTexture.begin(), reflectionTexture.end());

        // return a mesh object created from the extracted mesh data
        //���ش���ȡ���������ݴ������������
        return Mesh(vertices, indices, textures);
    }


    //���������͵����в������������δ�����������������
    //������Ϣ��Ϊ����ṹ���ء�
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;//��������
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            //���֮ǰ�Ƿ��������������ǣ��������һ�ε�������������������
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);//������һ���µ�Ԫ�ؼӵ�vector������棬λ��Ϊ��ǰ���һ��Ԫ�ص���һ��Ԫ��
                    //�Ѽ��ؾ�����ͬ�ļ�·���������������һ������(�Ż���
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   
                //���������δ���أ��������
                // if texture hasn't been loaded already, load it
                Texture texture;
                //��ȡ����Ӧ������ID��
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);//��ĩ�˲���Ԫ�����ṩǿ�����ı�֤
                //����洢Ϊ����ģ�ͼ��ص�������ȷ�����᲻��Ҫ�ؼ����ظ�����
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};

//ͼ���ȡ
//---------
unsigned char* Image_1(const char* path, int* widthCout, int* height,int a = 0, int b = 0) {
    return stbi_load(path, widthCout, height, &a, 0);
}

typedef struct {
    GLsizei widthCout;
    GLsizei height;
    GLenum format;
    GLint internalFormat;
    GLuint id;
    GLubyte* texels;
}gl_texture_t;
gl_texture_t* ReadPNGFromFile(const char* filename);
GLuint loadPNGTexture(const char* filename);

//���ļ��ж�ȡ����
//----------------
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    int widthCout, height, nrComponents;
    unsigned char* data =  stbi_load(filename.c_str(), &widthCout, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 3;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = 3;

        std::cout << "Texture find to load at path: " << format << std::endl;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, widthCout, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //�ͷ�����
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif