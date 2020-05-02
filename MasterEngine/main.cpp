#include "EditorManager.h"
#include "ScenesManager.h"
#include "RenderManager.h"
#include "GuiManager.h"
#include "GameManager.h"
#include "PhysicsManager.h"
#include "TextureManager.h"
#include "PrefabManager.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "TimeManager.h"

int main()
{
    SET_LOG_TYPE(McEngine::LogType::DBG);
    McEngine::Time::TimeManager::getInstance().start();
    McEngine::Files::FileManager::getInstance().start();
    McEngine::Editor::EditorManager::getInstance().start();
    McEngine::GameWindow::WindowManager::getInstance().start();
    McEngine::Scenes::ScenesManager::getInstace().start();
    McEngine::Renderer::RenderManager::getInstance().start();
    McEngine::Meshes::PrefabManager::getInstance().start();
    McEngine::Shaders::ShaderManager::getInstance().start();
    McEngine::Textures::TextureManager::getInstance().start();
    McEngine::Editor::EditorManager::getInstance().start();
    McEngine::Inputs::InputManager::getInstance().start();
    McEngine::Gui::GuiManager::getInstance().start();

    //-----------------------------------------------------------//
    //                     Editor Main Loop                      //
    McEngine::Editor::EditorManager::getInstance().editorLoop(); //
    //                                                           //
    //-----------------------------------------------------------//

    McEngine::Gui::GuiManager::getInstance().shutdown();
    McEngine::Inputs::InputManager::getInstance().shutdown();
    McEngine::Editor::EditorManager::getInstance().shutdown();
    McEngine::Textures::TextureManager::getInstance().shutdown();
    McEngine::Shaders::ShaderManager::getInstance().shutdown();
    McEngine::Meshes::PrefabManager::getInstance().shutdown();
    McEngine::Renderer::RenderManager::getInstance().shutdown();
    McEngine::Scenes::ScenesManager::getInstace().shutdown();
    McEngine::GameWindow::WindowManager::getInstance().shutdown();
    McEngine::Editor::EditorManager::getInstance().shutdown();
    McEngine::Files::FileManager::getInstance().shutdown();
    McEngine::Time::TimeManager::getInstance().shutdown();

    return 0;
}