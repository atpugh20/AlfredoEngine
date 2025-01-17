#pragma once

#include <macros.h>

#include "GLAD/glad.h"
#include "stb/stb_image.h"

#include <string>

class Texture {
private:
    unsigned int id;
    std::string filePath;
    unsigned char *localBuffer;
    int width, height, BPP;
public:
    Texture(std::string filePath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() { return width; }
    inline int getHeight() { return height; }
};
