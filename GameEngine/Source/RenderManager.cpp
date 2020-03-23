#include "RenderManager.h"
#include "ScenesManager.h"
#include "GuiManager.h"

namespace McEngine
{
namespace Renderer
{

void RenderManager::start()
{
    Scenes::ScenesManager::getInstace().getScenes().at(0)->getWindow().setCurrentContext();
    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("Failed to initialize GLAD", LogType::ERR);
        exit(0);
    }
}

void RenderManager::shutdown()
{
    
}

void RenderManager::draw(Scenes::Scene & p_scene)
{
    auto& objectManager = p_scene.getObjectManager();
    p_scene.getWindow().poolEvents();
    glClearColor(p_scene.m_backgroundColor.x, 
                 p_scene.m_backgroundColor.y, 
                 p_scene.m_backgroundColor.z, 
                 p_scene.m_backgroundColor.w);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto& object : objectManager.getObjects())
    {
        object.first.m_vertexArray->bindVao();
        object.first.m_shaderProgram->bindShaderProgram();

        objectManager.processObject(object.first);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        object.first.m_shaderProgram->unbindShaderProgram();
        object.first.m_vertexArray->unbindVao();
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    p_scene.getWindow().swapBuffer();
}

void RenderManager::initGlad()
{
    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("Failed to initialize GLAD", LogType::ERR);
        exit(0);
    }
}

}//Renderer
}//McEngine