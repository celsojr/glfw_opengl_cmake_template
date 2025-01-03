#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

class Shader final
{
public:
    unsigned int ID;
    Shader() : ID(0) {}
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    ~Shader();

    GLuint compileShader(const char *source, GLenum shaderType);
    // GLuint createShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

    void use() const;
    // void setBool(const std::string &name, bool value) const;
    // void setInt(const std::string &name, int value) const;
    // void setFloat(const std::string &name, float value) const;
    // void setVec2(const std::string &name, const glm::vec2 &value) const;
    // void setVec3(const std::string &name, const glm::vec3 &value) const;
    // void setVec4(const std::string &name, const glm::vec4 &value) const;
    // void setMat2(const std::string &name, const glm::mat2 &value) const;
    // void setMat3(const std::string &name, const glm::mat3 &value) const;
    // void setMat4(const std::string &name, const glm::mat4 &value) const;

private:
    GLuint program;
    // checks if compilation or linking failed and if so, print the error logs
    // void checkCompileErrors(unsigned int object, std::string type);
};

#endif // SHADER_H
