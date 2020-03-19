#include "ScenesManager.h"
#include "Logger.h"

namespace McEngine
{
namespace Scenes
{

void ScenesManager::start()
{
    LOG("Prepare Scene", LogType::INF);
    m_scenes.push_back(std::make_unique<Scene>());
    m_scenes.at(0)->prepareScene();
}

void ScenesManager::shutdown()
{
    m_scenes.clear();
}

std::vector<std::unique_ptr<Scene>>& ScenesManager::getScenes()
{
    return m_scenes;
}

}//Scenes
}//McEngine
