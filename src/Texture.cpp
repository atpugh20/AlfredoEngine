#include "Texture.h"

Texture::Texture(std::string filePath) 
    : id(0), localBuffer(nullptr), width(0), height(0), BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate texture
    localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);
    if (localBuffer) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(localBuffer);
    } else {
        printError("Image failed to load.");
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
