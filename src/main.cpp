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

#include <cmath>

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

glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window) {
    const float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main(void) {
        
    const float w_W = 800.0f;
    const float w_H = 600.0f;

    // Create Window
    GLFWwindow* window = createWindow(w_W, w_H, "Alfredo Engine");
    if (!window) 
        return -1;

    glEnable(GL_DEPTH_TEST);
   
    const int floatsPerVertex   = 8;
    const float off              = 0.5f;  // Distance each vertex is offset from the center point

    // Vertex and index Data
    float vertices[] = {
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

    int indices[] = {
        0,  1,  2,     2,  3,  0,  // front
        4,  5,  6,     6,  7,  4,  // back
        8,  9, 10,    10, 11,  8,  // left
       12, 13, 14,    14, 15, 12,  // right
       16, 17, 18,    18, 19, 16,  // top
       20, 21, 22,    22, 23, 20,  // bottom
    };
    const int iCount = sizeof(indices) / sizeof(indices[0]);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    VertexArray* VAO = new VertexArray();
    VertexBuffer* VBO = new VertexBuffer();
    ElementBuffer *EBO = new ElementBuffer();
    Texture *texture = new Texture("res/textures/diamond.png");
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

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 mvp;

    const float radius = 20.0f;

    // Rotation of cubes
    float degrees = 0.1f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        degrees += 20.0f * deltaTime;

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), w_W / w_H, 0.1f, 100.0f);

        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i + degrees), glm::vec3(1.0f, 0.3f, 0.5f));  
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