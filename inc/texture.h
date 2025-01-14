#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture final
{
private:
    GLuint textureID;
    static void getTextureCoords(int spriteIndex, float &texStartX, float &texEndX);
public:
    Texture() = delete;
    Texture(const std::string &filepath);
    ~Texture();
    static void renderSprite(int spriteIndex, int w_width, int w_height);
    void bind() const;
    void unbind() const;
};

#endif // TEXTURE_H