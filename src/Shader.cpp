#include "glad/glad.h"
#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();

    glShaderSource(vs, 1, &vertexShaderSource, nullptr);
    glShaderSource(fs, 1, &fragmentShaderSource, nullptr);

    glCompileShader(vs);
    glCompileShader(fs);
}

Shader::~Shader() {
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

void Shader::bind() const {
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
}

// Getters

unsigned int Shader::getVertexID() const { return vs; }

unsigned int Shader::getFragmentID() const { return fs; }

unsigned int Shader::getProgram() const { return program; }

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
