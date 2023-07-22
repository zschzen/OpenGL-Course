#include "../Public/Shader.h"

// initialize static map of shaders
std::map<std::string, GLuint> Shader::uniformLocations = std::map<std::string, GLuint>();
std::vector<Shader*> Shader::shaders = std::vector<Shader*>();

Shader::Shader()
{
    shaderID = 0;
}

Shader::~Shader()
{
    // Remove from static map of shaders
    shaders.erase(std::remove(shaders.begin(), shaders.end(), this), shaders.end());

    Clear();
}

Shader::Shader(const char* vertexLocation, const char* fragmentLocation)
{
    CreateFromFiles(vertexLocation, fragmentLocation);

    // Add to static map of shaders
    shaders.push_back(this);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);

    // Add to static map of shaders
    shaders.push_back(this);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
    fileLocation = GetAbsolutePath(fileLocation).c_str();

    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        //return;
    }
}

GLuint Shader::GetUniformLocation(const char* name)
{
    if (IsCached(name)) return uniformLocations[name];

    GLuint location = glGetUniformLocation(shaderID, name);
    uniformLocations[name] = location;
    return location;
}

void Shader::SetBool(const char* name, bool value)
{
    SetInt(name, static_cast<int>(value));
}

void Shader::SetInt(const char* name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const char* name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec3(const char* name, glm::vec3 value)
{
    glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const char* name, float x, float y, float z)
{
    glUniform3f(GetUniformLocation(name), x, y, z);
}

void Shader::SetVec4(const char* name, glm::vec4 value)
{
    glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const char* name, float x, float y, float z, float w)
{
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetMat4(const char* name, glm::mat4 value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Clear()
{
    if (shaderID == 0) return;
    glDeleteProgram(shaderID);
    shaderID = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

std::string Shader::GetAbsolutePath(const char* fileLocation)
{
    std::filesystem::path path(fileLocation);
    if (!path.is_absolute()) {
        path = std::filesystem::absolute(path);
    }
    return path.generic_string();
}

void Shader::SetGlobalBool(const char* name, bool value)
{
    for (auto& shader : shaders)
    {
        shader->SetBool(name, value);
    }
}

void Shader::SetGlobalInt(const char* name, int value)
{
    for (auto& shader : shaders)
    {
        shader->SetInt(name, value);
    }
}

void Shader::SetGlobalFloat(const char* name, float value)
{
    for (auto& shader : shaders)
    {
        shader->SetFloat(name, value);
    }
}

void Shader::SetGlobalVec3(const char* name, glm::vec3 value)
{
    for (auto& shader : shaders)
    {
        shader->SetVec3(name, value);
    }
}

void Shader::SetGlobalVec3(const char* name, float x, float y, float z)
{
    for (auto& shader : shaders)
    {
        shader->SetVec3(name, x, y, z);
    }
}

void Shader::SetGlobalVec4(const char* name, glm::vec4 value)
{
    for (auto& shader : shaders)
    {
        shader->SetVec4(name, value);
    }
}

void Shader::SetGlobalVec4(const char* name, float x, float y, float z, float w)
{
    for (auto& shader : shaders)
    {
        shader->SetVec4(name, x, y, z, w);
    }
}

void Shader::SetGlobalMat4(const char* name, glm::mat4 value)
{
    for (auto& shader : shaders)
    {
        shader->SetMat4(name, value);
    }
}
