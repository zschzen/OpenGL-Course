#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

    std::string ReadFile(const char* fileLocation);

    inline GLuint GetProjectionLocation() { return uniformProjection; }
    inline GLuint GetModelLocation() { return uniformModel; }
    inline GLuint GetViewLocation() { return uniformView; }

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID = 0;
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

private:
    std::string GetAbsolutePath(const char* fileLocation);
};