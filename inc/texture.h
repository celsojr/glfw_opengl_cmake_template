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

public:
    Texture() = delete;
    Texture(const std::string &filepath);
    ~Texture();
    void bind() const;
    void unbind() const;
};

#endif // TEXTURE_H