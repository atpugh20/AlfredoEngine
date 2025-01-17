#pragma once

#include "glm/glm.hpp"

#include <string>

class Shader {
private:
    unsigned int vs;
    unsigned int fs;
    unsigned int program;

public:
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~Shader();
    
    void bind() const;

    unsigned int getVertexID() const;
    unsigned int getFragmentID() const;
    unsigned int getProgram() const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
