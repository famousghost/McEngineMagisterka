#include "ShaderManager.h"
#include "WindowManager.h"
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
    addShader("diffuseShader", "Shaders/lightCube.vlsl", "Shaders/lightCube.flsl");
    addShader("textureShader", "Shaders/textureCube.vlsl", "Shaders/textureCube.flsl");
    addShader("customObjectShader", "Shaders/textureCustom.vlsl", "Shaders/textureCustom.flsl");
    addShader("windowShader", "Shaders/windowShader.vlsl", "Shaders/windowShader.flsl");
    addShader("coliderShader", "Shaders/colider.vlsl", "Shaders/colider.flsl");
    addShader("lightShader", "Shaders/light.vlsl", "Shaders/light.flsl");
    GameWindow::WindowManager::getInstance().createWindowPlane();
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

void ShaderManager::resetShaders()
{
    m_shaders.clear();
    addShader("defaultShader", "Shaders/vertexShader.vlsl", "Shaders/fragmentShader.flsl");
    addShader("colorShader", "Shaders/colorCube.vlsl", "Shaders/colorCube.flsl");
    addShader("diffuseShader", "Shaders/lightCube.vlsl", "Shaders/lightCube.flsl");
    addShader("textureShader", "Shaders/textureCube.vlsl", "Shaders/textureCube.flsl");
    addShader("customObjectShader", "Shaders/textureCustom.vlsl", "Shaders/textureCustom.flsl");
    addShader("windowShader", "Shaders/windowShader.vlsl", "Shaders/windowShader.flsl");
    addShader("coliderShader", "Shaders/colider.vlsl", "Shaders/colider.flsl");
    addShader("lightShader", "Shaders/light.vlsl", "Shaders/light.flsl");
    GameWindow::WindowManager::getInstance().createWindowPlane();
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& p_label)
{
    try
    {
        return m_shaders.at(p_label);
    }
    catch (std::exception& ex)
    {
        LOG("Cannot find shader program", LogType::ERR);
        return nullptr;
    }
}

}//Shaders
}//GameEngine