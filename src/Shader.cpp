#include "glad/glad.h"
#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vertexShaderSource, nullptr);
    glShaderSource(fs, 1, &fragmentShaderSource, nullptr);

    glCompileShader(vs);
    glCompileShader(fs);
}

Shader::~Shader() {
    glDeleteShader(vs);
    glDeleteShader(fs);
}

unsigned int Shader::getProgram() {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    return program;
}