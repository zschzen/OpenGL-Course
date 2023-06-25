#include "../Public/ModelLoader.h"

#include "../Public/MeshData.h"

MeshData ModelLoader::LoadModel(const std::string &fileName)
{
    MeshData modelData;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_TransformUVCoords);

    if (!scene)
    {
        fprintf(stderr, "Failed to load model: %s\n", importer.GetErrorString());
        return modelData;
    }

    // Process each mesh in the loaded model
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[i];
        ProcessMesh(mesh, scene, modelData);
    }

    return modelData;
}

void ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene, MeshData &modelData)
{
    // Process each vertex in the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        modelData.vertices.push_back(mesh->mVertices[i].x);
        modelData.vertices.push_back(mesh->mVertices[i].y);
        modelData.vertices.push_back(mesh->mVertices[i].z);
    }

    // Process each face in the mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            modelData.indices.push_back(face.mIndices[j]);
        }
    }
}
