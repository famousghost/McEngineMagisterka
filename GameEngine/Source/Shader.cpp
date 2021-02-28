#include "Shader.h"
#include "ShaderLoader.h"
#include "Logger.h"

namespace McEngine
{
namespace Shaders
{

Shader::~Shader()
{
    glDeleteShader(m_vertexShaderId);
    glDeleteShader(m_fragmentShaderId);
    glDeleteProgram(m_shaderProgramId);
}

void Shader::compileShader(GLuint p_shaderId, const std::string& p_shaderPath)
{
    ShaderLoader l_shaderLoader;
    std::string l_shaderSource = l_shaderLoader.loadShaderFromFile(p_shaderPath);
    const char * l_source = l_shaderSource.c_str();
    glShaderSource(p_shaderId, 1, &l_source, NULL);

    glCompileShader(p_shaderId);
}

bool Shader::getShaderCompileStatus(GLuint p_shaderId)
{
    char l_errBuffer[512];
    int l_success;

    glGetShaderiv(p_shaderId, GL_COMPILE_STATUS, &l_success);

    if (not l_success)
    {
        glGetShaderInfoLog(p_shaderId, 512, NULL, l_errBuffer);
        std::string l_errMessage = l_errBuffer;
        LOG("Cannot compile shader:" + l_errMessage, LogType::WARN);
        return false;
    }
    return true;
}

GLuint Shader::createShader(const std::string& p_shaderPath, GLenum p_shaderType)
{
    GLuint l_shaderId = glCreateShader(p_shaderType);

    compileShader(l_shaderId, p_shaderPath);
    if (not getShaderCompileStatus(l_shaderId))
    {
        return -1;
    }

    return l_shaderId;
}

bool Shader::getProgramLinkStatus()
{
    char l_errBuffer[512];
    int l_success;

    glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &l_success);

    if (not l_success)
    {
        glGetProgramInfoLog(m_shaderProgramId, 512, NULL, l_errBuffer);
        std::string l_errMessage = l_errBuffer;
        LOG("Cannot link shader program:" + l_errMessage, LogType::WARN);
        return false;
    }
    return true;
}

void Shader::attachShadersToProgram()
{
    glAttachShader(m_shaderProgramId, m_vertexShaderId);
    glAttachShader(m_shaderProgramId, m_fragmentShaderId);

    glLinkProgram(m_shaderProgramId);
}

bool Shader::createProgram(const std::string& p_vertexShaderPath, const std::string& p_fragmentShaderPath)
{
    m_vertexShaderPath = p_vertexShaderPath;
    m_fragmentShaderPath = p_fragmentShaderPath;

    m_vertexShaderId = createShader(m_vertexShaderPath, GL_VERTEX_SHADER);
    m_fragmentShaderId = createShader(m_fragmentShaderPath, GL_FRAGMENT_SHADER);

    if (m_vertexShaderId == -1 or m_fragmentShaderId == -1)
    {
        return false;
    }

    m_shaderProgramId = glCreateProgram();

    attachShadersToProgram();
    if (not getProgramLinkStatus())
    {
        return false;
    }
    return true;
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

void Shader::uniform1I(GLuint p_value, const std::string & p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniform1i(l_uniformLocationId, p_value);
}

void Shader::uniform1f(float p_value, const std::string & p_uniformName)
{
    GLuint l_uniformLocationId = glGetUniformLocation(m_shaderProgramId, p_uniformName.c_str());
    glUniform1f(l_uniformLocationId, p_value);
}

GLuint Shader::getShaderProgramId() const
{
    return m_shaderProgramId;
}

void Shader::refresh()
{
    compileShader(m_vertexShaderId, m_vertexShaderPath);
    getShaderCompileStatus(m_vertexShaderId);
    compileShader(m_fragmentShaderId, m_fragmentShaderPath);
    getShaderCompileStatus(m_fragmentShaderId);
    attachShadersToProgram();
    getProgramLinkStatus();
}

}//Shaders
}//GameEngine

