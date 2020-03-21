#include "ShaderManager.h"
#include <algorithm>
#include "Logger.h"

namespace McEngine
{
namespace Shaders
{

ShaderManager & ShaderManager::getInstance()
{
    static ShaderManager shaderManager;
    return shaderManager;
}

void ShaderManager::start()
{
    addShader("defaultShader", "Shaders/vertexShader.vlsl", "Shaders/fragmentShader.flsl");
    addShader("colorShader", "Shaders/colorCube.vlsl", "Shaders/colorCube.flsl");
}

void ShaderManager::shutdown()
{
    m_shaders.clear();
}

void ShaderManager::addShader(std::string p_shaderLabel,
                              std::string p_vertexShader,
                              std::string P_fragmentShader)
{
    std::shared_ptr<Shader> l_shader = std::make_shared<Shader>();
    l_shader->createProgram(p_vertexShader, P_fragmentShader);
    m_shaders.insert(std::make_pair(p_shaderLabel, l_shader));
}

std::shared_ptr<Shader> ShaderManager::getShader(std::string p_label) const
{
    return m_shaders.at(p_label);
}

}//Shaders
}//GameEngine