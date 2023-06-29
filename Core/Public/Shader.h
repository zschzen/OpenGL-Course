#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

    std::string ReadFile(const char* fileLocation);

    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);
    void SetVec3(const char* name, glm::vec3 value);
    void SetMat4(const char* name, glm::mat4 value);

    // Getters
    inline GLuint GetUniformLocation(const char* name) const { return glGetUniformLocation(shaderID, name); }

    void Use();
    void Clear();

    ~Shader();

private:
    GLuint shaderID = 0;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

private:
    std::string GetAbsolutePath(const char* fileLocation);
};