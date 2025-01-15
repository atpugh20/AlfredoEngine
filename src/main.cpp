#include "macros.h"
#include "Window.h"

#include "stb_image/stb_image.h"

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

const char* vsSource = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
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

int main(void) {

    // Create Window
    GLFWwindow* window = createWindow(600, 600, "Alfredo Engine");
    if (!window) 
        return -1;
   
    const int floatsPerVertex   = 8;
    const float off              = 0.5f;  // Distance each vertex is offset from the center point

    // Vertex and index Data
    float vertices[] = {
        // Positions        // Colors           // Texture coords
        -off,  off,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,         // Top left
         off,  off,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,         // Top right
         off, -off,  0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,         // Bot right
        -off, -off,  0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,         // Bot left
    };

    int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    const int iCount = sizeof(indices) / sizeof(indices[0]);

    Texture *texture = new Texture("res/textures/torchic.png");
    VertexArray* VAO = new VertexArray();
    VertexBuffer* VBO = new VertexBuffer();
    ElementBuffer *EBO = new ElementBuffer();

    VAO->bind();
    VBO->bind(vertices, sizeof(vertices));
    EBO->bind(indices, sizeof(indices));

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Shaders
    Shader *shader = new Shader(vsSource, fsSource);
    unsigned int program = shader->getProgram();
    delete shader;

    float xInc = 0.001f;
    int xIndex[] = {
        0, 8, 16, 24
    };

    while (!glfwWindowShouldClose(window)) {
        for (int x : xIndex) {
            if (vertices[x] > 1 || vertices[x] < -1) {
                xInc *= -1;
                break;
            }
        }

        //for (int x : xIndex) vertices[x] += xInc;

        // Render 
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        
        VAO->bind();
        VBO->bind(vertices, sizeof(vertices));

        glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete texture;
    delete VAO;
    delete VBO;
    delete EBO;

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}