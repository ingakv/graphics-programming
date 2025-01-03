#include "Shader.h"


Shader::Shader(const std::string &lab_name, const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc) {
    auto vSrc = readFromShader(vertexShaderSrc + "VSSrc", lab_name);
    auto fSrc = readFromShader(fragmentShaderSrc + "FSSrc", lab_name);

    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(VertexShader, reinterpret_cast<const GLchar *const *>(&vSrc));

    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(FragmentShader, reinterpret_cast<const GLchar *const *>(&fSrc));

    ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);

    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

Shader::Shader(const std::string &lab_name, const std::string &shaderSrc) :
Shader(lab_name, shaderSrc, shaderSrc) {}

Shader::~Shader() {
    glDeleteShader(ShaderProgram);
}

void Shader::Bind() const {
    glUseProgram(ShaderProgram);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::UploadUniformInt(const std::string& name, const int& value) const {
    glUniform1i(GetLocation(name), value);
}

//Send data from matrices to uniform
void Shader::UploadUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformFloat(const std::string& name, const float& value) const {
    glUniform1f(GetLocation(name), value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const{
    glUniform2f(GetLocation(name), vector.x, vector.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const{
    glUniform3f(GetLocation(name), vector.x, vector.y, vector.z);
}

// Get the location of the uniform in your shader program
int Shader::GetLocation(const std::string &name) const {
    return glGetUniformLocation(ShaderProgram, name.c_str());
}

void Shader::CompileShader(GLenum shaderType, const GLchar *const *shaderSrc) {
    glShaderSource(shaderType, 1, shaderSrc, nullptr);
    glCompileShader(shaderType);
}

// Converts a glsl file into a string
std::string Shader::readFromShader(const std::string& shaderSrc, const std::string& lab_name) {

    std::string filepath;
    std::string shaderSource;
    std::string line;
    std::string lab = lab_name;

    // If it is a lab, the filepath will be updated to include the labs subfolder
    if (lab_name.find("labs") != std::string::npos) {
        filepath = "labs";
        lab = "";
    }
    else if (lab_name.find("lab") != std::string::npos) filepath = "labs/";

    std::ifstream file("../../" + filepath + lab +"/resources/shaders/" + shaderSrc + ".glsl");

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
    }
    else {
        // Reads the glsl file line by line and combines it into a string
        while (std::getline(file, line)) {
            shaderSource += line + "\n";
        }

        file.close();

    }

    // Now, shaderSource contains the GLSL code as a string.
    return shaderSource;
}
