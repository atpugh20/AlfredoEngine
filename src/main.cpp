#include "macros.h"
#include "Window.h"

#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Camera.h"
#include "Perlin.hpp"

#include <cmath>
#include <cstdlib>
#include <vector>

const char* vsSource = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 TexCoord;

uniform mat4 mvp;

void main() {
    gl_Position =  mvp * vec4(aPos, 1.0);
    vColor = aColor;
    TexCoord = aTexCoord;
}
)";

const char* fsSource = R"(
#version 460 core
out vec4 FragColor;

in vec3 vColor;
in vec2 TexCoord;

uniform sampler2D vTexture;

void main() {
    FragColor = texture(vTexture, TexCoord) * vec4(vColor, 1.0);
}
)";

void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const float w_W = 800.0f;  // Window width
const float w_H = 600.0f;  // Window height

Camera camera(glm::vec3(5.0f, 10.0f, 15.0f));

bool firstMouse = true;
float lastX     = w_W / 2.0;
float lastY     = w_H / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main(void) {
    srand(time(0));
    const siv::PerlinNoise::seed_type seed = rand() % 99999;
    const siv::PerlinNoise perlin{ seed };
    
    // Create Window
    GLFWwindow* window = createWindow(w_W, w_H, "Alfredo Engine");
    if (!window) 
        return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   
    const int floatsPerVertex   = 8;
    const float off             = 0.5f;  // Distance each vertex is offset from the center point

    // Vertex and index Data
    float vertices[] = {
        // Positions       // Colors           // Texture coords
        // front
        -off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
         off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
         off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
        -off, -off,  off,   0.5f, 0.5f, 0.5f,   0.0f,  0.0f,     // Bot left
        // back
         off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
        -off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
        -off, -off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
         off, -off, -off,   0.5f, 0.5f, 0.5f,   0.0f,  0.0f,     // Bot left
        // left
        -off,  off, -off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
        -off,  off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
        -off, -off,  off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
        -off, -off, -off,   0.5f, 0.5f, 0.5f,   0.0f,  0.0f,     // Bot left        
        // right
         off,  off,  off,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,     // Top left
         off,  off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  1.0f,     // Top right
         off, -off, -off,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,     // Bot right
         off, -off,  off,   0.5f, 0.5f, 0.5f,   0.0f,  0.0f,     // Bot left
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

    int indices[] = {
        0,  1,  2,     2,  3,  0,  // front
        4,  5,  6,     6,  7,  4,  // back
        8,  9, 10,    10, 11,  8,  // left
       12, 13, 14,    14, 15, 12,  // right
       16, 17, 18,    18, 19, 16,  // top
       20, 21, 22,    22, 23, 20,  // bottom
    };
    const int iCount = sizeof(indices) / sizeof(indices[0]);

    std::vector<glm::vec3> cubePositions;

    int xMax = 10;
    int yMax = 10;
    int zMax = 10;
    float noiseOff = 0.2f;

    for (float x = 0.0f; x < xMax; x++) {
        for (float y = 0.0f; y < yMax; y++) {
            for (float z = 0.0f; z < zMax; z++) {
                const double noise = perlin.noise3D_01(x * noiseOff, y * noiseOff, z * noiseOff);
                if (noise < 0.5 || y < 1)
                    cubePositions.push_back(glm::vec3(x, y, z));
            }
        }
    }

    VertexArray* VAO = new VertexArray();
    VertexBuffer* VBO = new VertexBuffer();
    ElementBuffer *EBO = new ElementBuffer();
    Texture *texture = new Texture("res/textures/stone.png");
    Shader *shader = new Shader(vsSource, fsSource);

    // Bind everything to GPU
    VAO->bind();
    VBO->bind(vertices, sizeof(vertices));
    EBO->bind(indices, sizeof(indices));
    texture->bind(0);
    shader->bind();

    unsigned int program = shader->getProgram();

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Camera
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 mvp;

    const float radius = 20.0f;

    // Rotation of cubes
    float degrees = 0.1f;

    while (!glfwWindowShouldClose(window)) {

        // Calculate deltaTime
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        print(1 / deltaTime);

        processInput(window);

        glClearColor(0.53f, 0.8f, 0.92f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the window between frames

        degrees += 20.0f * deltaTime;
        
        // Calculate Model View Projection
        view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        projection = glm::perspective(glm::radians(camera.Zoom), w_W / w_H, 0.1f, 100.0f);
        for (unsigned int i = 0; i < cubePositions.size(); i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            //model = glm::rotate(model, glm::radians(20.0f * i + degrees), glm::vec3(1.0f, 0.3f, 0.5f));  
            mvp = projection * view * model;
            shader->setMat4("mvp", mvp);
            glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);
        }
        
        glUseProgram(program);
        VAO->bind();
        VBO->bind(vertices, sizeof(vertices));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete VAO;
    delete VBO;
    delete EBO;
    delete texture;
    delete shader;

    glfwTerminate();
    return 0;
}
    
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
