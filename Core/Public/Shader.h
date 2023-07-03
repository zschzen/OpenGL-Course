#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>

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

    inline void Use() { glUseProgram(shaderID); }
    void Clear();

    ~Shader();

public:
    // Get cached uniform location. If not cached, cache it.
    GLuint GetUniformLocation(const char* name);

private:
    GLuint shaderID = 0;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

private:
    std::string GetAbsolutePath(const char* fileLocation);

    // Map of all uniform locations
    std::map<std::string, GLuint> uniformLocations = std::map<std::string, GLuint>();

    // Check if a uniform location is already cached
    inline bool IsCached(const char* name) const { return uniformLocations.find(name) != uniformLocations.end(); }
};