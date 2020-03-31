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
    m_cameras.emplace_back(new Cameras::Camera);
    m_scenes.at(0)->prepareScene();
    m_currentCamera = m_cameras.at(0);
    m_currentAvaibaleScene = m_scenes.at(0);
}

void ScenesManager::updateCurrentScene(std::string sceneLabel)
{
    //TODO add scene vector as pair with label and object scene
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

std::shared_ptr<Cameras::Camera>& ScenesManager::getCurrentAvaiableCamera()
{
    return m_currentCamera;
}

}//Scenes
}//McEngine
