#pragma once

class Texture {
private:
    unsigned int id;
    unsigned char *localBuffer;
    int width, height, BPP;
public:
    Texture();
    ~Texture();

    void bind() const;
    void unbind() const;

    inline int getWidth() { return width; }
    inline int getHeight() { return height; }
};
