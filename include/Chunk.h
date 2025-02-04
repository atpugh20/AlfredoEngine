#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Cube.h"
#include "Perlin.hpp"
#include "Shader.h"

#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <format>


class Chunk {
private:
    siv::PerlinNoise::seed_type seed = rand() % 99999;
    siv::PerlinNoise perlin{ seed };

    glm::mat4 model;
    int cubeCount;

    int xMax = 16;
    int yMax = 16;
    int zMax = 16;
    float noiseOff = 0.2f;
    float off = 0.5f;
    

public:

    std::unordered_map<std::string, Cube> cubes;

    std::string last;  // The last block filled. Used for binding the buffer.

    Chunk() {
        Fill();
    }

    void Fill() {
        cubes.clear();
        /*siv::PerlinNoise::seed_type seed = rand() % 99999;
        siv::PerlinNoise perlin{ seed };*/


        for (int x = 0.0f; x < xMax; x++) {
            for (int y = 0.0f; y < yMax; y++) {
                for (int z = 0.0f; z < zMax; z++) {
                    const double noise = perlin.noise3D_01(x * noiseOff, y * noiseOff, z * noiseOff);
                    if (noise < 0.5 || y < 1) { 
                        last = std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
                        cubes[last] = Cube(x, y, z, off);
                    }
                }
            }
        }

        cubeCount = cubes.size();
    }

    void Draw(glm::mat4 view, glm::mat4 projection, glm::mat4& mvp, Shader* shader) {
        
        // Try to batch together all vertices

        for (auto &pair : cubes) {
            if (pair.second.WillDraw) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, cubes[pair.first].position);
                //model = glm::rotate(model, glm::radians(20.0f * i + degrees), glm::vec3(1.0f, 0.3f, 0.5f));  
                mvp = projection * view * model;
                shader->setMat4("mvp", mvp);
                cubes[pair.first].draw();
            }
        }
    }

};
