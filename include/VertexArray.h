#pragma once
#include "glad/glad.h"

class VertexArray {
private:
    unsigned int id;

public:
    VertexArray() {
        glGenVertexArrays(1, &id);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &id);
    }

    void bind() const {
        glBindVertexArray(id);
    }

    void unbind() const {
        glBindVertexArray(0);
    }
};
