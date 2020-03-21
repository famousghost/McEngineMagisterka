#pragma once
#include "Scene.h"
#include <vector>
#include <memory>

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

private:
    ScenesManager() = default;
    ~ScenesManager() = default;
    std::vector<std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_currentAvaibaleScene;
};
}//Scenes
}//McEngine

