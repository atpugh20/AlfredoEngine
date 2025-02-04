#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>

#include "glm/glm.hpp"

class Cube {
public: 
    //std::string texturePath;
    std::vector<float> vertices;
    std::vector<int> indices;
    glm::vec3 position;

    int x;
    int y; 
    int z;

    bool WillDraw;

    Cube() : x(0), y(0), z(0), WillDraw(false), position(glm::vec3(0,0,0)), vertices{}, indices{} {}

    Cube(int x, int y, int z, const float off)
        :
        x(x), y(y), z(z),
        WillDraw(false),
        position(glm::vec3(x, y, z)),
        vertices{
            // Positions       // Colors           // Texture coords
            // front
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off,  off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
            -off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // back
             off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
            -off, -off, -off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
             off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // left
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
            -off, -off,  off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
            -off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left        
            // right
             off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off, -off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
             off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // top
            -off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off,  off,  off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
            -off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
            // bottom
            -off, -off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
             off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
             off, -off, -off,   0.75f, 0.75f, 0.75f,   1.0f,  0.0f,     // Bot right
            -off, -off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f,     // Bot left
        },

        indices{
            0,  1,  2,     2,  3,  0,  // front
            4,  5,  6,     6,  7,  4,  // back
            8,  9, 10,    10, 11,  8,  // left
           12, 13, 14,    14, 15, 12,  // right
           16, 17, 18,    18, 19, 16,  // top
           20, 21, 22,    22, 23, 20,  // bottom
        } 
    {}
    

    void draw() {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
   
    void checkSurrounding(const std::unordered_map<std::string, Cube>& cubes) {
        char buffer1[50];
        char buffer2[50];
        char buffer3[50];
        char buffer4[50];
        char buffer5[50];
        char buffer6[50];

        std::snprintf(buffer1, sizeof(buffer1), "%d,%d,%d", x + 1, y, z);
        std::snprintf(buffer2, sizeof(buffer2), "%d,%d,%d", x - 1, y, z);
        std::snprintf(buffer3, sizeof(buffer3), "%d,%d,%d", x, y + 1, z);
        std::snprintf(buffer4, sizeof(buffer4), "%d,%d,%d", x, y - 1, z);
        std::snprintf(buffer5, sizeof(buffer5), "%d,%d,%d", x, y, z + 1);
        std::snprintf(buffer6, sizeof(buffer6), "%d,%d,%d", x, y, z - 1);

        if (cubes.contains(buffer1) &&
            cubes.contains(buffer2) &&
            cubes.contains(buffer3) &&
            cubes.contains(buffer4) &&
            cubes.contains(buffer5) &&
            cubes.contains(buffer6) ) 
        {
            WillDraw = false;
        } else {
            WillDraw = true;
        }
    }
};
