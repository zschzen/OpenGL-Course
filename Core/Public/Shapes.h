#pragma once

#include "MeshData.h"
#include <cmath>

class Shapes
{
public:
    // Return a cube
    static MeshData Cube()
    {
        MeshData cube;

        // Vertices
        cube.vertices = {
            // Front
            -0.5f, -0.5f, 0.5f, // 0
            0.5f, -0.5f, 0.5f,  // 1
            0.5f, 0.5f, 0.5f,   // 2
            -0.5f, 0.5f, 0.5f,  // 3
            // Back
            -0.5f, -0.5f, -0.5f, // 4
            0.5f, -0.5f, -0.5f,  // 5
            0.5f, 0.5f, -0.5f,   // 6
            -0.5f, 0.5f, -0.5f   // 7
        };

        // Indices
        cube.indices = {
            // Front
            0, 1, 2,
            2, 3, 0,
            // Right
            1, 5, 6,
            6, 2, 1,
            // Back
            7, 6, 5,
            5, 4, 7,
            // Left
            4, 0, 3,
            3, 7, 4,
            // Bottom
            4, 5, 1,
            1, 0, 4,
            // Top
            3, 2, 6,
            6, 7, 3};

        return cube;
    }

    // Return a sphere
    static MeshData Sphere(int segments = 32, int rows = 16)
    {
        MeshData sphere;

        for (int row = 0; row <= rows; row++)
        {
            float v = row / (float)rows;
            float phi = v * glm::pi<float>();

            for (int segment = 0; segment <= segments; segment++)
            {
                float u = segment / (float)segments;
                float theta = u * glm::two_pi<float>();

                float x = cos(theta) * sin(phi);
                float y = cos(phi);
                float z = sin(theta) * sin(phi);

                sphere.vertices.push_back(x);
                sphere.vertices.push_back(y);
                sphere.vertices.push_back(z);
            }
        }

        for (int row = 0; row < rows; row++)
        {
            for (int segment = 0; segment < segments; segment++)
            {
                int first = (row * (segments + 1)) + segment;
                int second = first + segments + 1;

                sphere.indices.push_back(first);
                sphere.indices.push_back(second);
                sphere.indices.push_back(first + 1);

                sphere.indices.push_back(second);
                sphere.indices.push_back(second + 1);
                sphere.indices.push_back(first + 1);
            }
        }
        return sphere;
    }

    // Return a pyramid
    static MeshData Pyramid()
    {
        MeshData pyramid;

        // Vertices
        pyramid.vertices = {
            // Base
            -0.5f, 0.0f, -0.5f, // 0
            0.5f, 0.0f, -0.5f,  // 1
            0.5f, 0.0f, 0.5f,   // 2
            -0.5f, 0.0f, 0.5f,  // 3
                                // Apex
            0.0f, 1.0f, 0.0f    // 4
        };

        // Indices
        pyramid.indices = {
            // Base
            0, 1, 2,
            2, 3, 0,
            // Sides
            4, 0, 1,
            4, 1, 2,
            4, 2, 3,
            4, 3, 0};

        return pyramid;
    }
};
