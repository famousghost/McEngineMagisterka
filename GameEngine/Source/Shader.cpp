#include "Shader.h"
#include "ShaderLoader.h"
#include "Logger.h"

namespace McEngine
{
namespace Shaders
{

Shader::~Shader()
{
    glDeleteProgram(m_shaderProgramId);
}

GLuint Shader::createShader(std::string p_shaderPath, GLenum p_shaderType)
{
    ShaderLoader l_shaderLoader;
    std::string l_shaderSource = l_shaderLoader.loadShaderFromFile(p_shaderPath);
    GLuint l_shaderId = glCreateShader(p_shaderType);
    const char * l_source = l_shaderSource.c_str();
    glShaderSource(l_shaderId, 1, &l_source, NULL);

    glCompileShader(l_shaderId);

    char l_errBuffer[512];
    int l_success;

    glGetShaderiv(l_shaderId, GL_COMPILE_STATUS, &l_success);

    if (not l_success)
    {
        glGetShaderInfoLog(l_shaderId, 512, NULL, l_errBuffer);
        std::string l_errMessage = l_errBuffer;
        LOG("Cannot compile shader:" + l_errMessage, LogType::WARN);
    }

    return l_shaderId;
}

void Shader::createProgram(std::string p_vertexShaderPath, std::string p_fragmentShaderPath)
{
    GLuint l_vertexShaderId = createShader(p_vertexShaderPath, GL_VERTEX_SHADER);
    GLuint l_fragmentShaderId = createShader(p_fragmentShaderPath, GL_FRAGMENT_SHADER);

    m_shaderProgramId = glCreateProgram();

    glAttachShader(m_shaderProgramId, l_vertexShaderId);
    glAttachShader(m_shaderProgramId, l_fragmentShaderId);

    glLinkProgram(m_shaderProgramId);

    char l_errBuffer[512];
    int l_success;

    glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &l_success);

    if (not l_success)
    {
        glGetProgramInfoLog(m_shaderProgramId, 512, NULL, l_errBuffer);
        std::string l_errMessage = l_errBuffer;
        LOG("Cannot link shader program:" + l_errMessage, LogType::WARN);
    }

    glDeleteShader(l_vertexShaderId);
    glDeleteShader(l_fragmentShaderId);
}

void Shader::bindShaderProgram()
{
    glUseProgram(m_shaderProgramId);
}

void Shader::unbindShaderProgram()
{
    glUseProgram(0);
}

void Shader::uniformMatrix4(const glm::mat4& p_matrix, const std::string& p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniformMatrix4fv(l_uniformLocationId, 1, GL_FALSE, glm::value_ptr(p_matrix));
}

void Shader::uniformMatrix3(const glm::mat3& p_matrix, const std::string& p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniformMatrix4fv(l_uniformLocationId, 1, GL_FALSE, glm::value_ptr(p_matrix));
}

void Shader::uniformVec4(const glm::vec4& p_vec, const std::string& p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniform4fv(l_uniformLocationId, 1, glm::value_ptr(p_vec));
}

void Shader::uniformVec3(const glm::vec3& p_vec, const std::string& p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniform3fv(l_uniformLocationId, 1, glm::value_ptr(p_vec));
}

void Shader::uniform1I(GLuint p_textureUniformId, const std::string & p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniform1i(l_uniformLocationId, p_textureUniformId);
}

}//Shaders
}//GameEngine

