#pragma once
#ifndef SHADER_H
#define SHADER_H

//#include <glad/glad.h>
//#include <glm/glm.hpp>
//
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>

class Shader
{
public:
    void chushihua(void) {

#pragma region GLAD初始化

        //GLAD初始化,用来管理OpenGL函数指针的，调用任何OpenGL的函数之前都需要初始化GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            //返回初始化失败信息
            Print::Line("Failed to initialize GLAD");
            //return -1;
        }

#pragma endregion

    }

    GLuint ID;
    Shader()
    {
        ID = 0;
    }
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(string vertexPath, string fragmentPath, string geometryPath = "")
    {
        activate(vertexPath, fragmentPath, geometryPath);
    }

    void activate(string vertexPath, string fragmentPath, string geometryPath = "")
    {
        //chushihua();
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            //打开文件// open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            //读取文件相应内容//read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件句柄//close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // 赋值流内容到string中//convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            //如果存在几何体着色器路径，同时加载几何体着色器// if geometry shader path is present, also load a geometry shader
            if (geometryPath != "")
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            Print::Line("错误：着色器无法正确读取文件" + vertexPath +fragmentPath); //ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 编译着色器//2. compile shaders
        //unsigned int vertex, fragment;
        // 顶点着色器//vertex shader
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // 片段着色器//fragment Shader
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        //如果给定了几何体着色器，编译几何体着色器// if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != "")
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        //为着色器程序附加上顶点着色器
        glAttachShader(ID, vertex);
        //为着色器程序附加上片段着色器
        glAttachShader(ID, fragment);
        if (geometryPath != "")
            ////为着色器程序附加上几何体着色器
            glAttachShader(ID, geometry);
        //链接着色器对象
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        //删除着色器，因为它们现在链接到我们的程序中，不再需要// delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != "")
            glDeleteShader(geometry);

    }

    // 激活着色器//activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // 多功能的uniform函数//utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    //用于检查着色器编译/链接错误的实用程序函数。// utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                char* s = infoLog;
                Print::Line("错误：：着色器编译错误, 类型： " + type + "\n" + infoLog + "\n-- -------------------------------------------------- - -- ");//ERROR::SHADER_COMPILATION_ERROR of type :
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                char* s = infoLog;
                Print::Line("错误：：程序链接错误,类型：" + type + "\n" + infoLog + "\n-- -------------------------------------------------- - -- ");//ERROR::PROGRAM_LINKING_ERROR of type: 
            }
        }
    }
};
#endif