#include "EditorManager.h"
#include "ScenesManager.h"
#include "RenderManager.h"
#include "GuiManager.h"
#include "GameManager.h"
#include "PhysicsManager.h"
#include "TextureManager.h"
#include "VAOManager.h"
#include "ShaderManager.h"

int main()
{
    McEngine::Editor::EditorManager::getInstance().start();
    McEngine::Scenes::ScenesManager::getInstace().start();
    McEngine::Renderer::RenderManager::getInstance().start();
    McEngine::Meshes::VAOManager::getInstance().start();
    McEngine::Shaders::ShaderManager::getInstance().start();
    McEngine::Gui::GuiManager::getInstance().start();

    //-----------------------------------------------------------//
    //                        Main Loop                          //
    McEngine::Editor::EditorManager::getInstance().editorLoop();
    //                                                           //
    //-----------------------------------------------------------//

    McEngine::Gui::GuiManager::getInstance().shutdown();
    McEngine::Shaders::ShaderManager::getInstance().shutdown();
    McEngine::Meshes::VAOManager::getInstance().shutdown();
    McEngine::Renderer::RenderManager::getInstance().shutdown();
    McEngine::Scenes::ScenesManager::getInstace().shutdown();
    McEngine::Editor::EditorManager::getInstance().shutdown();
    return 0;
}