#include "../Public/Skybox.h"

#include <stb/stb_image.h>

#include "../Public/Shapes.h"

Skybox::Skybox() {
}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
    // Shader Setup
    skyShader = new Shader();
    skyShader->CreateFromFiles("Assets/Shaders/Skybox/skybox.vert", "Assets/Shaders/Skybox/skybox.frag");

    uniformProjection = skyShader->GetUniformLocation("projection");
    uniformView = skyShader->GetUniformLocation("view");

    // Texture Setup
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int width, height, bitDepth;

    for (size_t i = 0; i < 6; i++) {
        unsigned char *texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
        if (!texData) {
            printf("Failed to find: %s\n", faceLocations[i].c_str());
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texData);
        stbi_image_free(texData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Mesh Setup
    skyMesh = new Mesh(Shapes::Cube(1.f));

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void Skybox::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    // Remove translation from the view matrix
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));

    // Disable depth writing
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    skyShader->Use();

    skyShader->SetMat4("view", viewMatrix);
    skyShader->SetMat4("projection", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    // Render the skybox
    skyMesh->RenderMesh();

    // Re-enable depth writing and face culling for other objects
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

Skybox::~Skybox() {
}

