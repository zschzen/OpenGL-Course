#ifndef __SHAPES_H
#define __SHAPES_H

#include <vector>

#include "MeshData.h"

class Shapes
{
public:
    static MeshData Quad(float width = 1.f, float height = 1.f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Calculate half of width and height
        const float w2 = width / 2.f;
        const float h2 = height / 2.f;

        // Create vertices
        vertices.emplace_back(glm::vec3(-w2, -h2, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f));
        vertices.emplace_back(glm::vec3(w2, -h2, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f));
        vertices.emplace_back(glm::vec3(w2, h2, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f));
        vertices.emplace_back(glm::vec3(-w2, h2, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 1.f));

        // Create indices
        indices.emplace_back(0);
        indices.emplace_back(1);
        indices.emplace_back(2);
        indices.emplace_back(2);
        indices.emplace_back(3);
        indices.emplace_back(0);

        std::vector<Texture> textures{}; // You can add textures here if needed

        return {vertices, indices, textures};
    }

    static MeshData Sphere(float radius = 1.f, int numSegments = 16.f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= numSegments; ++i)
        {
            const float phi = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(numSegments);
            for (int j = 0; j <= numSegments; ++j)
            {
                const float theta = glm::two_pi<float>() * static_cast<float>(j) / static_cast<float>(numSegments);
                float x = radius * std::sin(phi) * std::cos(theta);
                float y = radius * std::cos(phi);
                float z = radius * std::sin(phi) * std::sin(theta);
                glm::vec3 pos(x, y, z);
                glm::vec3 norm = glm::normalize(pos);

                // Calculate texture coordinates based on spherical coordinates
                float u = static_cast<float>(j) / static_cast<float>(numSegments);
                float v = static_cast<float>(i) / static_cast<float>(numSegments);
                glm::vec2 tex(u, v);

                vertices.emplace_back(pos, norm, tex);

                // Add indices if we're not at the first row or first vertex of the current row
                if (i > 0 && j > 0)
                {
                    unsigned int a = (i - 1) * (numSegments + 1) + j - 1;
                    unsigned int b = (i - 1) * (numSegments + 1) + j;
                    unsigned int c = i * (numSegments + 1) + j;
                    unsigned int d = i * (numSegments + 1) + j - 1;
                    indices.push_back(a);
                    indices.push_back(b);
                    indices.push_back(c);
                    indices.push_back(c);
                    indices.push_back(d);
                    indices.push_back(a);
                }
            }
        }

        std::vector<Texture> textures; // You can add textures here if needed

        return {vertices, indices, textures};
    }

    static MeshData Cube(float size = 1.f)
    {
        float halfSize = size / 2.0f;

        std::vector<Vertex> vertices = {
                // Front face
                Vertex(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, -halfSize, halfSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, halfSize, halfSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-halfSize, halfSize, halfSize), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),

                // Back face
                Vertex(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(halfSize, halfSize, -halfSize), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),

                // Left face
                Vertex(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(-halfSize, halfSize, halfSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

                // Right face
                Vertex(glm::vec3(halfSize, -halfSize, halfSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, halfSize, -halfSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(halfSize, halfSize, halfSize), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

                // Top face
                Vertex(glm::vec3(-halfSize, halfSize, halfSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, halfSize, halfSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, halfSize, -halfSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-halfSize, halfSize, -halfSize), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),

                // Bottom face
                Vertex(glm::vec3(-halfSize, -halfSize, -halfSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, -halfSize, -halfSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(halfSize, -halfSize, halfSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-halfSize, -halfSize, halfSize), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
        };

        std::vector<unsigned int> indices = {
                // Front face
                0, 1, 2, 2, 3, 0,
                // Back face
                4, 5, 6, 6, 7, 4,
                // Left face
                8, 9, 10, 10, 11, 8,
                // Right face
                12, 13, 14, 14, 15, 12,
                // Top face
                16, 17, 18, 18, 19, 16,
                // Bottom face
                20, 21, 22, 22, 23, 20,
        };

        std::vector<Texture> textures; // You can add textures here if needed

        return {vertices, indices, textures};
    }

    static MeshData Cone(float radius = 1.f, float height = 2.f, int numSegments = 16)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Create the vertex at the tip of the cone (apex)
        vertices.emplace_back(glm::vec3(0.f, height * 0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.5f, 0.f));

        for (int i = 0; i <= numSegments; ++i)
        {
            const float theta = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(numSegments);
            float x = radius * std::cos(theta);
            float z = radius * std::sin(theta);
            glm::vec3 pos(x, -height * 0.5f, z);
            glm::vec3 norm = glm::normalize(glm::vec3(x, 0.f, z));

            // Calculate texture coordinates based on circular cross-section
            float u = static_cast<float>(i) / static_cast<float>(numSegments);
            glm::vec2 tex(u, 1.f);

            vertices.emplace_back(pos, norm, tex);

            // Connect the vertex to the apex
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back((i + 1) % numSegments + 1);
        }

        std::vector<Texture> textures; // You can add textures here if needed

        return {vertices, indices, textures};
    }

    static MeshData Torus(float majorRadius = 1.f, float minorRadius = 0.5f, int numMajorSegments = 16.f, int numMinorSegments = 16.f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= numMajorSegments; ++i)
        {
            const float phi = glm::two_pi<float>() * static_cast<float>(i) / static_cast<float>(numMajorSegments);
            for (int j = 0; j <= numMinorSegments; ++j)
            {
                const float theta = glm::two_pi<float>() * static_cast<float>(j) / static_cast<float>(numMinorSegments);
                float x = (majorRadius + minorRadius * cos(theta)) * cos(phi);
                float y = (majorRadius + minorRadius * cos(theta)) * sin(phi);
                float z = minorRadius * sin(theta);
                glm::vec3 pos(x, y, z);

                // Calculate texture coordinates based on spherical coordinates
                float u = static_cast<float>(j) / static_cast<float>(numMinorSegments);
                float v = static_cast<float>(i) / static_cast<float>(numMajorSegments);
                glm::vec2 tex(u, v);

                // Calculate normal vector using cross product of tangent vectors
                glm::vec3 du(-sin(phi), cos(phi), 0.f);
                glm::vec3 dv(-sin(theta) * cos(phi), -sin(theta) * sin(phi), cos(theta));
                glm::vec3 norm = glm::normalize(glm::cross(du, dv));

                vertices.emplace_back(pos, norm, tex);

                // Add indices if we're not at the first row or first vertex of the current row
                if (i > 0 && j > 0)
                {
                    unsigned int a = (i - 1) * (numMinorSegments + 1) + j - 1;
                    unsigned int b = (i - 1) * (numMinorSegments + 1) + j;
                    unsigned int c = i * (numMinorSegments + 1) + j;
                    unsigned int d = i * (numMinorSegments + 1) + j - 1;
                    indices.push_back(a);
                    indices.push_back(b);
                    indices.push_back(c);
                    indices.push_back(c);
                    indices.push_back(d);
                    indices.push_back(a);
                }
            }
        }

        std::vector<Texture> textures{}; // You can add textures here if needed

        return {vertices, indices, textures};
    }

};

#endif //__SHAPES_H
