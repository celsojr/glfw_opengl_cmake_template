#include "texture.h"

Texture::Texture(const std::string &filepath)
{
    glGenTextures(1, &textureID);

    std::cout << "Loading texture from: " << filepath << std::endl;

    // Set stb_image to flip loaded textures on the y-axis
    // Should called before stbi_load
    stbi_set_flip_vertically_on_load(true);

    // Load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cerr << "Failed to load texture from: " << filepath << std::endl;
    }
    stbi_image_free(data);

    if (textureID == 0)
    {
        std::cerr << "Failed to load texture" << std::endl;
        glfwTerminate();
        // return 0;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
