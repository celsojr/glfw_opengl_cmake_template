#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "shader.h"

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    std::string vertexShaderCode = Utils::readFile(vertexShaderPath);
    std::string fragmentShaderCode = Utils::readFile(fragmentShaderPath);

    if (vertexShaderCode.empty() || fragmentShaderCode.empty())
    {
        return;
    }

    GLuint vertexShader = compileShader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        return;
    }

    program = glCreateProgram();
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
        return;
    }

    ID = program;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Shader::compileShader(const char *source, GLenum shaderType)
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

void Shader::use() const
{
    glUseProgram(program);
}

Shader::~Shader()
{
    glDeleteProgram(program);
}
