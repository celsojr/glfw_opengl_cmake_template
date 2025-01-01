#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <sstream>
#include <GLFW/glfw3.h>

// Include stb_image for loading images
// https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebufferSizeChanged(GLFWwindow *window, int width, int height);
void render(GLFWwindow *window);
GLuint loadTexture(const std::string &filepath);
std::string readFile(const std::string &filepath);
GLuint compileShader(const char *source, GLenum shaderType);
GLuint createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

GLuint VAO, VBO, EBO, textureID;
GLuint shaderProgram;

const int W_WIDTH = 400, W_HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Minesweeper with OpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeChanged);

    shaderProgram = createShaderProgram(RESOURCES_DIR "/shaders/vertexShader.glsl", RESOURCES_DIR "/shaders/fragmentShader.glsl");

    // Set stb_image to flip loaded textures on the y-axis
    stbi_set_flip_vertically_on_load(true);

    // Define vertices and texture coordinates for a quad (will be scaled to texture size)
    // float aspectRatio = 384.0f / 32.0f;
    float scaleX = 384.0f / (float)W_WIDTH; // Width of texture relative to window width
    float scaleY = 32.0f / (float)W_HEIGHT; // Height of texture relative to window height

    float vertices[] = {
        // scale          // texture coords
        -scaleX, -scaleY, 0.0f, 0.0f, 0.0f, // bottom left
        scaleX, -scaleY, 0.0f, 1.0f, 0.0f,  // bottom right
        scaleX, scaleY, 0.0f, 1.0f, 1.0f,   // top right
        -scaleX, scaleY, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    textureID = loadTexture(RESOURCES_DIR "/textures/atlas.png");

    if (textureID == 0)
    {
        std::cerr << "Failed to load texture" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
    if (textureLocation == -1)
    {
        std::cerr << "Failed to get uniform location for 'ourTexture'" << std::endl;
    }
    glUniform1i(textureLocation, 0);

    while (!glfwWindowShouldClose(window))
    {
        render(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebufferSizeChanged(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint loadTexture(const std::string &filepath)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    std::cout << "Loading texture from: " << filepath << std::endl;

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

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture from: " << filepath << std::endl;
        return 0;
    }

    return textureID;
}

std::string readFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

GLuint compileShader(const char *source, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader Compilation Error:\n"
                  << infoLog << std::endl;
        return 0;
    }

    return shader;
}

GLuint createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexShaderCode = readFile(vertexShaderPath);
    std::string fragmentShaderCode = readFile(fragmentShaderPath);

    if (vertexShaderCode.empty() || fragmentShaderCode.empty())
    {
        return 0;
    }

    GLuint vertexShader = compileShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Shader Program Linking Error:\n"
                  << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void render(GLFWwindow *window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind texture and shader program
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
