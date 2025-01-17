#pragma once
#include <glad/glad.h>

class ElementBuffer {
private:
    unsigned int id;

public:
    ElementBuffer() {
        glGenBuffers(1, &id);
    }

    ~ElementBuffer() {
        glDeleteBuffers(1, &id);
    }

    void bind(const void *data, unsigned int size) const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    void unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};
