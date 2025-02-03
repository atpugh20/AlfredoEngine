#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"

class Cube {
public: 
    bool exists;
    std::string texturePath;
    std::vector<float> vertices;
    std::vector<int> indices;
    glm::vec3 position;

    Cube(float x, float y, float z, const float off, bool exists) {
        position = glm::vec3(x, y, z);
         
        vertices = {
            // Positions       // Colors           // Texture coords
            // front
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
            -off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // back
             off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
            -off, -off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
             off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // left
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
            -off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
            -off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left        
            // right
             off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
             off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // top
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // bottom
            -off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
            -off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
        };

        indices = {
            0,  1,  2,     2,  3,  0,  // front
            4,  5,  6,     6,  7,  4,  // back
            8,  9, 10,    10, 11,  8,  // left
           12, 13, 14,    14, 15, 12,  // right
           16, 17, 18,    18, 19, 16,  // top
           20, 21, 22,    22, 23, 20,  // bottom
        };
    }

    void draw() {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
};
