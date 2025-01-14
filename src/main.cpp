#include "macros.h"
#include "Window.h"

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

const char* vsSource = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vColor = aColor;
}
)";

const char* fsSource = R"(
#version 460 core
out vec4 FragColor;
in vec3 vColor;

void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

int main(void) {

    // Create Window
    GLFWwindow* window = createWindow(640, 480, "Alfredo Engine");
    if (!window) 
        return -1;
   
    const int floatsPerVertex   = 6;
    const float sL              = 0.5f;  // Length of each size of the triangle

    // Vertex and index Data
    float vertices[] = {
        0.0f,  0.0f, -1.0f,   0.0f, 1.0f, 0.0f,
        sL,    0.0f,  1.0f,   1.0f, 0.0f, 0.0f,
       -sL,    0.0f,  1.0f,   0.0f, 0.0f, 1.0f,
    };

    int indices[] = {
        0, 1, 2, 
    };
    const int iCount = sizeof(indices) / sizeof(indices[0]);

    float textureCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.5f, 1.0f,
    };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Buffers
    VertexArray* VAO = new VertexArray();
    VertexBuffer* VBO = new VertexBuffer();
    ElementBuffer *EBO = new ElementBuffer();

    VAO->bind();
    VBO->bind(vertices, sizeof(vertices));
    EBO->bind(indices, sizeof(indices));

    // Vertex Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shaders
    Shader *shader = new Shader(vsSource, fsSource);
    unsigned int program = shader->getProgram();
    delete shader;


    while (!glfwWindowShouldClose(window)) {
        
        // Render 
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        VAO->bind();
        VBO->bind(vertices, sizeof(vertices));

        glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete VAO;
    delete VBO;
    delete EBO;

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}