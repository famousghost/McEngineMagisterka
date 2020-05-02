#pragma once
#include "Camera.h"
#include "ObjectBuilder.h"
#include "ObjectManager.h"

namespace McEngine
{
namespace Scenes
{

class Scene
{
public:
    Scene();

    Meshes::ObjectManager& getObjectManager();

    glm::vec4 m_backgroundColor;

    std::shared_ptr<Cameras::Camera>& getEditorCamera();

    std::shared_ptr<Cameras::Camera>& getGameMainCamera();
private:
    Meshes::ObjectManager m_objectManager;
    
    std::shared_ptr<Cameras::Camera> m_editorCamera;
    std::shared_ptr<Cameras::Camera> m_gameMainCamera;
    std::vector<std::shared_ptr<Cameras::Camera>> m_cameras;
};
}//Scenes
}//McEngine

