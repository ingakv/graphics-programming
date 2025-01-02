#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>


class Shader
{
public:
    Shader(const std::string &lab_name, const std::string &shaderSrc);
    Shader(const std::string &lab_name, const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    int GetLocation(const std::string &name) const;
    void UploadUniformFloat(const std::string& name, const float& value) const;
    void UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
    void UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
    void UploadUniformInt(const std::string& name, const int& index) const;
    void UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const;

private:
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint ShaderProgram;

    void CompileShader(GLenum shaderType, const GLchar *const *shaderSrc);
    std::string readFromShader(const std::string& shaderSrc, const std::string& lab_name);
};



#endif // SHADER_H_