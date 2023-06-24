#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const GLfloat ASPECT_RATIO = (GLfloat)WIDTH / (GLfloat)HEIGHT;

// Camera settings
const float FOV = 45.0f;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 100.0f;

// Math constants
const float DEG_TO_RAD = glm::pi<float>() / 180.0f;

// Triangle data
GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

// Vertex shader
static const char *vertexShader = "                                     \n\
#version 330                                                            \n\
                                                                        \n\
out vec4 vCol;                                                          \n\
                                                                        \n\
layout (location = 0) in vec3 pos;                                      \n\
                                                                        \n\
uniform mat4 model;                                                     \n\
uniform mat4 projection;                                                \n\
                                                                        \n\
void main()                                                             \n\
{                                                                       \n\
    gl_Position = projection * model * vec4(pos, 1.0);                  \n\
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                          \n\
}";

// Fragment shader
static const char *fragmentShader = "                                   \n\
#version 330                                                            \n\
                                                                        \n\
in vec4 vCol;                                                           \n\
out vec4 tint;                                                          \n\
                                                                        \n\
void main()                                                             \n\
{                                                                       \n\
    tint = vCol;                                                        \n\
}";

void CreateTriangle()
{
    unsigned int indices[] = {
        0, 3, 1, // Right side
        1, 3, 2, // Left side
        2, 3, 0, // Front side
        0, 1, 2  // Base
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    // Generate vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Generate vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify vertex attributes
    // 0: Attribute id (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); // Enable id 0

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void AddShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    // Create shader object
    GLuint newShader = glCreateShader(shaderType);

    // Get shader source
    const GLchar *code[1];
    code[0] = shaderCode;

    // Get code length
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // Compile shader
    glShaderSource(newShader, 1, code, codeLength);
    glCompileShader(newShader);

    // Check for errors
    GLint result = 0;
    GLchar eLog[1024] = {0};

    // Get shader compile status
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        const char *shaderTypeStr = shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment";
        glGetShaderInfoLog(newShader, sizeof(eLog), NULL, eLog); // Get error log
        printf("Error compiling the %s shader: '%s'\n", shaderTypeStr, eLog);
        return;
    }

    // Attach shader to program
    glAttachShader(program, newShader);
}

void CompileShaders()
{
    // Create shader program
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Shader creation failed!");
        return;
    }

    // Add shaders
    AddShader(shader, vertexShader, GL_VERTEX_SHADER);
    AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    // Link shader program
    // Creates executables that will run on the GPU shaders
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    // Validate shader program
    // Checks if the executables are valid for the current OpenGL state
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        //return;
    }

    // Get uniform locations
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW initialization failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW
    // OpenGL 3.3 (MAJOR.MINOR)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set minor version to 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile (no backwards compatibility)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Allow forward compatibility

    // Create window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Udemy OpenGL Course", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW Window creation failed!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set GLEW context
    glfwMakeContextCurrent(mainWindow);

    // Allow modern experimental features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initalization failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Create objects and shaders
    CreateTriangle();
    CompileShaders();

    // Create projection matrix
    glm::mat4 projection = glm::perspective(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

    // Main loop
    do
    {
        // Get input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.16f, 0.16f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // RENDERING
        // ------------------------------

        // Use shader program
        glUseProgram(shader);

        // TRANSLATION
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, 45.0f * DEG_TO_RAD * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        // Set uniform variables
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind vertex array object
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        // Draw triangle
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        // Unbind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);

    } while (!glfwWindowShouldClose(mainWindow));

    return 0;
}