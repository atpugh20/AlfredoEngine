#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Cube.h"
#include "Perlin.hpp"
#include "Shader.h"

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <format>

class Chunk {
private:
    const siv::PerlinNoise::seed_type seed = rand() % 99999;
    const siv::PerlinNoise perlin{ seed };
    glm::mat4 model;
    int cubeCount;

    int xMax = 10;
    int yMax = 10;
    int zMax = 10;
    float noiseOff = 0.2f;
    float off = 0.5;

public:

    // Keep this as an unordered map with tuple as the key, then use the tuple to do a lookup
    // Then you only have to use the tuple for O(1) time complexity.
    std::unordered_map<std::tuple<int, int, int>, Cube> cubes;


    Chunk() {
        Fill();
    }

    void Fill() {
        cubes.clear();
        for (float x = 0.0f; x < xMax; x++) {
            for (float y = 0.0f; y < yMax; y++) {
                for (float z = 0.0f; z < zMax; z++) {
                    const double noise = perlin.noise3D_01(x * noiseOff, y * noiseOff, z * noiseOff);
                    std::string key = std::format("{},{},{}", x, y, z);
                    if (noise < 0.5 || y < 1)
                        cubes[key] = Cube(x, y, z, off, true);
                    else
                        cubes[key] = Cube(x, y, z, off, false);
                }
            }
        }
        cubeCount = cubes.size();
    }

    void Draw(glm::mat4 view, glm::mat4 projection, glm::mat4& mvp, Shader* shader) {
        for (auto &pair : cubes) {
            if (cubes[pair.first].exists) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, cubes[pair.first].position);
                //model = glm::rotate(model, glm::radians(20.0f * i + degrees), glm::vec3(1.0f, 0.3f, 0.5f));  
                mvp = projection * view * model;
                shader->setMat4("mvp", mvp);
                cubes[pair.first].draw();
            }
        }
    }

private:

    // Check if block is surrounded or not
    bool isSurrounded() {

    }

};
