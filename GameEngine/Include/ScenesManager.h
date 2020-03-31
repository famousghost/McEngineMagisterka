#pragma once
#include "Scene.h"
#include <vector>
#include <memory>
#include "Camera.h"

namespace McEngine
{
namespace Scenes
{
class ScenesManager
{
public:

    static ScenesManager& getInstace();

    void start();

    void shutdown();

    void updateCurrentScene(std::string sceneLabel);

    std::vector<std::shared_ptr<Scene>>& getScenes();

    std::shared_ptr<Scene>& getCurrentAvaiableScene();

    std::shared_ptr<Cameras::Camera>& getCurrentAvaiableCamera();

private:
    ScenesManager() = default;
    ~ScenesManager() = default;
    std::vector<std::shared_ptr<Scene>> m_scenes;
    std::vector<std::shared_ptr<Cameras::Camera>> m_cameras;
    std::shared_ptr<Scene> m_currentAvaibaleScene;
    std::shared_ptr<Cameras::Camera> m_currentCamera;
};
}//Scenes
}//McEngine

