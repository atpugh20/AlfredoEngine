#pragma once
#include "glad/glad.h"

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer() {
        glGenBuffers(1, &id);
    }
    ~VertexBuffer() {
        glDeleteBuffers(1, &id);
    }

    void bind(const void *data, unsigned int size) const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 
    }
    void unbind() const {  
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
