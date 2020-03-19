#pragma once
#include "Window.h"
#include "ObjectBuilder.h"
#include "ObjectManager.h"

namespace McEngine
{
namespace Scenes
{

class Scene
{
public:
    void prepareScene();

    GameWindow::Window& getWindow();

    Meshes::ObjectManager& getObjectManager();

    glm::vec4 m_backgroundColor;
private:
    GameWindow::Window m_window;
    Meshes::ObjectManager m_objectManager;
};
}//Scenes
}//McEngine

