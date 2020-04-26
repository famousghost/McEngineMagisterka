#include "Scene.h"
#include "CubeBuilder.h"

namespace McEngine
{
namespace Scenes
{
namespace
{
    constexpr uint32_t WIDTH = 1920;
    constexpr uint32_t HEIGHT = 1080;
}

void Scene::prepareScene()
{
    m_backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    m_window.createWindow("McEngine", WIDTH, HEIGHT);
}

GameWindow::Window& Scene::getWindow()
{
    return m_window;
}

Meshes::ObjectManager& Scene::getObjectManager()
{
    return m_objectManager;
}

}//Scenes
}//McEngine
