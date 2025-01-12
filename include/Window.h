#pragma once

#define GLEW_STATIC

#include "GLEW/glew.h"
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

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        glfwTerminate();
        printError("Glew not ok");
        return nullptr;
    }

    return window;
}
