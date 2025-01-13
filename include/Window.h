#pragma once

#define GLEW_STATIC

#include "glad/glad.h"
#include "GLFW/glfw3.h"


static GLFWwindow* createWindow(int width, int height, const char* title) { 
    GLFWwindow *window;
    if (!glfwInit()) {
        printError("GLFW failed to initialize.");
        return nullptr;
    }

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        printError("GLFW failed to create the window");
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
        printError("Failed to initialize GLAD");
        return nullptr;
    }

    return window;
}
