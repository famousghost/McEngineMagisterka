#include "EditorManager.h"
#include "Logger.h"
#include "Shader.h"
#include "ObjectManager.h"
#include "ScenesManager.h"
#include "RenderManager.h"
#include "GuiManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include <vector>

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
    double l_currentTime = 0.0;
    while (l_window.isOpen())
    {
        double l_previousTime = l_currentTime;
        l_currentTime = glfwGetTime();
        Time::TimeManager::getInstance().setDeltaTime(l_currentTime - l_previousTime);
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