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
    //模型数据
    // model data 
    //存储迄今为止加载的所有纹理，并进行优化以确保纹理不会加载多次。
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    //构造函数，需要指向三维模型的文件路径。
    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    //绘制模型及其所有网格
    // draws the model, and thus all its meshes
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    //从文件加载具有支持的ASIMP扩展名的模型，并将生成的网格存储在网格向量中。
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path)
    {
        //通过ASSIMP读取文件
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        //检查错误
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        //检索文件路径的目录路径
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        //递归处理ASSIMP的根节点
        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    //以递归方式处理节点。处理位于节点上的每个单独网格，并在其子节点（如果有）上重复此过程。
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        //处理位于当前节点的每个网格
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            //节点对象仅包含索引以索引场景中的实际对象。
            //场景包含了所有的数据，节点只是为了使内容保持有序（比如节点之间的关系）。
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //处理完所有网格（如果有）后，我们将递归处理每个子节点
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

        //遍历网格的每个顶点
        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            //我们声明一个占位符向量，因为assimp使用它自己的向量类，它不会直接转换为glm的vec3类，所以我们首先将数据传输到此占位符glm:：vec3。
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            //位置
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            //归一化
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            //纹理坐标
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?//网格是否包含纹理坐标？
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                ////一个顶点最多可以包含8个不同的纹理坐标。因此，我们假设我们不会
                //使用一个顶点可以有多个纹理坐标的模型，因此我们始终使用第一组（0）。
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent//正切（切线）
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent//双切线
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        //现在遍历网格的每个面（面是网格及其三角形）并检索相应的顶点索引。
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            //检索面的所有索引并将其存储在索引向量中
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        //加工材质
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        //我们假设着色器中的采样器名称有一个约定。应命名每个漫反射纹理
        //作为“纹理扩散”，其中N是从1到最大采样器编号的序列号。
        //以下列表总结的内容同样适用于其他纹理：
        //漫反射：纹理\漫反射
        //镜面反射：纹理\镜面反射
        //法线：纹理\法线

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_diffuse");//aiTextureType_AMBIENT//aiTextureType_DIFFUSE
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");//aiTextureType_SPECULAR
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");//aiTextureType_HEIGHT,Assimp的aiTextureType_NORMAL并不会加载它的法线贴图，而aiTextureType_HEIGHT却能
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        //  5.reflection maps//环境映射
        std::vector<Texture> reflectionTexture = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
        textures.insert(textures.end(), reflectionTexture.begin(), reflectionTexture.end());

        // return a mesh object created from the extracted mesh data
        //返回从提取的网格数据创建的网格对象
        return Mesh(vertices, indices, textures);
    }


    //检查给定类型的所有材质纹理，如果尚未加载纹理，则加载纹理。
    //所需信息作为纹理结构返回。
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;//纹理数组
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            //检查之前是否加载了纹理，如果是，请继续下一次迭代：跳过加载新纹理
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);//函数将一个新的元素加到vector的最后面，位置为当前最后一个元素的下一个元素
                    //已加载具有相同文件路径的纹理，请继续下一个纹理(优化）
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   
                //如果纹理尚未加载，请加载它
                // if texture hasn't been loaded already, load it
                Texture texture;
                //获取到相应的纹理ID号
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);//在末端插入元件，提供强有力的保证
                //将其存储为整个模型加载的纹理，以确保不会不必要地加载重复纹理。
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};

//图像读取
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

//由文件中读取纹理
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

        //释放数据
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