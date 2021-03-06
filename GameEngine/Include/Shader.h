#pragma once
#include "glad.h"
#include <string>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
namespace McEngine
{
namespace Shaders
{

class Shader
{
public:
    Shader() = default;
    ~Shader();
    bool createProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath);
    void bindShaderProgram();
    void unbindShaderProgram();
    void uniformMatrix4(const glm::mat4& p_matrix, const std::string& p_uniformName);
    void uniformMatrix3(const glm::mat3 & p_matrix, const std::string& p_uniformName);
    void uniformVec4(const glm::vec4 & p_vec, const std::string& p_uniformName);
    void uniformVec3(const glm::vec3 & p_vec, const std::string& p_uniformName);
    void uniform1I(GLuint p_textureUniformId, const std::string& p_uniformName);
    void uniform1f(float p_value, const std::string& p_uniformName);

    GLuint getShaderProgramId() const;

    void refresh();
private:
    GLuint createShader(const std::string& p_shaderPath, GLenum p_shaderType);
    bool getProgramLinkStatus();
    void attachShadersToProgram();
    void compileShader(GLuint p_shaderId, const std::string & p_shaderPath);
    bool getShaderCompileStatus(GLuint p_shaderId);

    std::string m_vertexShader;
    std::string m_fragmentShader;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;

    GLuint m_shaderProgramId;
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
};

}//Shaders
}//GameEngine

