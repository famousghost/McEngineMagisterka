#include "EditorManager.h"
#include "Logger.h"
#include <vector>
#include "Shader.h"
#include "CubeBuilder.h"
#include "CubeModelBuilder.h"
#include "ObjectManager.h"
#include "ScenesManager.h"
#include "RenderManager.h"
#include "GuiManager.h"

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
    SET_LOG_TYPE(McEngine::LogType::DBG);
    LOG("Application start", McEngine::LogType::INF);
    std::vector<std::string> objectsName;

    auto& scenes = Scenes::ScenesManager::getInstace().getScenes();

    ImVec4 l_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glEnable(GL_DEPTH_TEST);

    while (scenes.at(0)->getWindow().isOpen())
    {
        Gui::GuiManager::getInstance().meshGui(l_clearColor);

        Renderer::RenderManager::getInstance().draw(*scenes.at(0));
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