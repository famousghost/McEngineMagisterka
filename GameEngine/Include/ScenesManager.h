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

    static ScenesManager& getInstace()
    {
        static ScenesManager sceneManager;
        return sceneManager;
    }

    void start();

    void shutdown();

    std::vector<std::unique_ptr<Scene>>& getScenes();

private:
    ScenesManager() = default;
    ~ScenesManager() = default;
    std::vector<std::unique_ptr<Scene>> m_scenes;
};
}//Scenes
}//McEngine

