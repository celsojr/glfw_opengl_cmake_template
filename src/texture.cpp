#include "texture.h"

// Constants for texture atlas
const float spriteWidth = 32.0f;
const float atlasWidth = 384.0f; // Width of the entire texture atlas
const float spriteHeight = 32.0f;
const float atlasHeight = 32.0f; // Assuming 1 row in the atlas

void Texture::getTextureCoords(int spriteIndex, float &texStartX, float &texEndX)
{
    texStartX = (spriteIndex * spriteWidth) / atlasWidth;
    texEndX = ((spriteIndex + 1) * spriteWidth) / atlasWidth;
}

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

void Texture::renderSprite(int spriteIndex, int w_width, int w_height)
{
    float texStartX, texEndX;
    getTextureCoords(spriteIndex, texStartX, texEndX);

    const float scaleX = 32.0f / (float)w_width;  // Width of texture relative to window width
    const float scaleY = 32.0f / (float)w_height; // Height of texture relative to window height

    // Update vertex array with calculated texture coordinates
    float vertices[] = {
        // scale          // texture coords
        -scaleX, -scaleY, 0.0f, texStartX, 0.0f, // bottom left
        scaleX, -scaleY, 0.0f, texEndX, 0.0f,    // bottom right
        scaleX, scaleY, 0.0f, texEndX, 1.0f,     // top right
        -scaleX, scaleY, 0.0f, texStartX, 1.0f   // top left
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Draw the sprite
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
