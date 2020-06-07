#include "ScenesManager.h"
#include "Logger.h"

namespace McEngine
{
namespace Scenes
{

ScenesManager& ScenesManager::getInstace()
{
    static ScenesManager sceneManager;
    return sceneManager;
}

void ScenesManager::start()
{
    LOG("Prepare Scene", LogType::INF);
    m_scenes.emplace_back(new Scene);
    m_currentAvaibaleScene = m_scenes.at(0);
    m_currentAvaibaleScene->getObjectManager().addSkyBox();
}

void ScenesManager::updateCurrentScene(std::string sceneLabel)
{
    //TODO add map of scenes
}

void ScenesManager::shutdown()
{
    m_scenes.clear();
}

std::vector<std::shared_ptr<Scene>>& ScenesManager::getScenes()
{
    return m_scenes;
}

std::shared_ptr<Scene>& ScenesManager::getCurrentAvaiableScene()
{
    return m_currentAvaibaleScene;
}

}//Scenes
}//McEngine
