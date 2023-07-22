#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader();
    Shader(const char* vertexLocation, const char* fragmentLocation);

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

    std::string ReadFile(const char* fileLocation);

    void SetBool(const char* name, bool value);
    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);
    void SetVec3(const char* name, glm::vec3 value);
    void SetVec3(const char* name, float x, float y, float z);
    void SetVec4(const char* name, glm::vec4 value);
    void SetVec4(const char* name, float x, float y, float z, float w);
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

public:
    // Globally set uniforms
    static void SetGlobalBool(const char* name, bool value);
    static void SetGlobalInt(const char* name, int value);
    static void SetGlobalFloat(const char* name, float value);
    static void SetGlobalVec3(const char* name, glm::vec3 value);
    static void SetGlobalVec3(const char* name, float x, float y, float z);
    static void SetGlobalVec4(const char* name, glm::vec4 value);
    static void SetGlobalVec4(const char* name, float x, float y, float z, float w);
    static void SetGlobalMat4(const char* name, glm::mat4 value);

private:
    // static list of all shaders
    static std::vector<Shader*> shaders;

    // Map of all uniform locations
    static std::map<std::string, GLuint> uniformLocations;

public:
    // Get the sahders in a list
    static std::vector<Shader*> GetShaders() { return shaders; }

    // Check if a uniform location is already cached
    static inline bool IsCached(const char* name) { return uniformLocations.find(name) != uniformLocations.end(); }
};