#include "Scene.h"

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
    m_cameras.emplace_back(new Cameras::Camera);
    m_shadowMapCamera = m_cameras.at(2);
    m_shadowMapCamera->setShadowMappnig();
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

std::shared_ptr<Cameras::Camera>& Scene::getShadowMapCamera()
{
    return m_shadowMapCamera;
}

}//Scenes
}//McEngine
