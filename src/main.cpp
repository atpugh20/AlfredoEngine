#include "macros.h"
#include "Window.h"

int main(void) {

    // Create Window
    GLFWwindow* window = createWindow(600, 600, "Alfredo Engine");
    if (!window) 
        return -1;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}