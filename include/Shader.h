#pragma once

class Shader {
private:
    unsigned int vs;
    unsigned int fs;

public:
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~Shader();

    unsigned int getProgram();
};
