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

Scene::Scene()
{
    m_cameras.emplace_back(new Cameras::Camera);
    m_editorCamera = m_cameras.at(0);
    m_cameras.emplace_back(new Cameras::Camera);
    m_gameMainCamera = m_cameras.at(1);
}

Meshes::ObjectManager& Scene::getObjectManager()
{
    return m_objectManager;
}

std::shared_ptr<Cameras::Camera>& Scene::getEditorCamera()
{
    return m_editorCamera;
}

std::shared_ptr<Cameras::Camera>& Scene::getGameMainCamera()
{
    return m_gameMainCamera;
}

}//Scenes
}//McEngine
