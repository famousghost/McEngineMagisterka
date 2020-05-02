#include "EditorManager.h"
#include "Logger.h"
#include <vector>
#include "Shader.h"
#include "CubeBuilder.h"
#include "ObjectManager.h"
#include "ScenesManager.h"
#include "RenderManager.h"
#include "GuiManager.h"
#include "WindowManager.h"

namespace McEngine
{
namespace Editor
{

void EditorManager::start()
{
    initGlfw();
}

void EditorManager::shutdown()
{
}

EditorManager & EditorManager::getInstance()
{
    static EditorManager editorManager;
    return editorManager;
}

void EditorManager::editorLoop()
{
    LOG("Application start", McEngine::LogType::INF);

    auto& scene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    auto& l_window = GameWindow::WindowManager::getInstance().getWindow();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    auto& l_renderManager = Renderer::RenderManager::getInstance();
    while (l_window.isOpen())
    {
        Gui::GuiManager::getInstance().meshGui();
        l_renderManager.draw(*scene);
    }

    LOG("Application end", McEngine::LogType::INF);
}

void EditorManager::initGlfw()
{
    if (not glfwInit())
    {
        LOG("Cannot init glfw", LogType::ERR);
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

}//Editor
}//McEngine