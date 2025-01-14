#include "stb_image.h"
#include "texture.h"
#include "shader.h"

void framebufferSizeChanged(GLFWwindow *window, int width, int height);
void render(GLFWwindow *window);

GLuint VAO, VBO, EBO;
Texture *texture;
Shader *shaderProgram;

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

    shaderProgram = new Shader(RESOURCES_DIR "/shaders/vertexShader.glsl", RESOURCES_DIR "/shaders/fragmentShader.glsl");

    int textureLocation = glGetUniformLocation(shaderProgram->ID, "ourTexture");
    if (textureLocation == -1)
    {
        std::cerr << "Failed to get uniform location for 'ourTexture'" << std::endl;
    }
    glUniform1i(textureLocation, 0);

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture = new Texture(RESOURCES_DIR "/textures/atlas.png");

    while (!glfwWindowShouldClose(window))
    {
        render(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebufferSizeChanged(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void render(GLFWwindow *window)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind shader program and texture
    shaderProgram->use();
    texture->bind();

    glBindVertexArray(VAO);
    Texture::renderSprite(10, W_WIDTH, W_HEIGHT);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
