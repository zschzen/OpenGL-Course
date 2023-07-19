#include "../Public/Model.h"

#include <stb/stb_image.h>

#include "../Public/Shader.h"
#include "../Public/MeshData.h"
#include "../Public/Frustum.h"

Model::Model() : Behaviour()
{
    aabb = std::make_unique<Vosgi::AABB>();
}

Model::Model(const char* path) : Behaviour()
{
    LoadModel(path);
    aabb = std::make_unique<Vosgi::AABB>(Vosgi::generateAABB(meshes));
}

Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    meshes.push_back(new Mesh(vertices, indices, textures));
    aabb = std::make_unique<Vosgi::AABB>(Vosgi::generateAABB(meshes));
}

Model::~Model()
{
    Clear();
}

void Model::Render(Shader& shader, unsigned int& draw)
{
    for (auto& mesh : meshes)
    {
        mesh->Draw(shader);
        draw++;
    }
}

void Model::Draw(const Frustum& frustum, Shader& shader, unsigned int& display, unsigned int& draw)
{
    if (!aabb->isOnFrustum(frustum, *transform)) return;

    shader.SetMat4("model", transform->GetModel());
    for (auto& mesh : meshes)
    {
        mesh->Draw(shader);
        draw++;
    }
    display++;
}

void Model::Clear()
{
    for (auto& mesh : meshes)
    {
        mesh->Clear();
        delete mesh;
    }
    meshes.clear();
    delete transform;
}

Vosgi::AABB* Model::GetWorldAABB()
{
    // Get global scale thanks to our transform
    const glm::vec3 globalCenter{transform->GetModel() * glm::vec4(aabb->center, 1.f)};

    // Scaled orientation
    const glm::vec3 right = transform->GetRight() * aabb->extents.x;
    const glm::vec3 up = transform->GetUp() * aabb->extents.y;
    const glm::vec3 forward = transform->GetForward() * aabb->extents.z;

    const float newIi = std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, right)) +
                        std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, up)) +
                        std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, forward));

    const float newIj = std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, right)) +
                        std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, up)) +
                        std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, forward));

    const float newIk = std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, right)) +
                        std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, up)) +
                        std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, forward));

    return new Vosgi::AABB(globalCenter, newIi, newIj, newIk);
}

void Model::LoadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs| aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
        return;
    }

    directory = fileName.substr(0, fileName.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

    texturesLoaded.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (const auto& loadedTexture : texturesLoaded)
        {
            if (std::strcmp(loadedTexture.path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTexture);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string fileName = std::string(path);
    fileName = directory + '/' + fileName;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        switch (nrComponents)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (gamma)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        printf("Texture failed to load at path: %s\n", path);
        stbi_image_free(data);
    }

    return textureID;
}
