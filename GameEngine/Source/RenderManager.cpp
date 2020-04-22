#include "RenderManager.h"
#include "ScenesManager.h"
#include "GuiManager.h"
#include "TextureManager.h"

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
    auto& l_camera = Scenes::ScenesManager::getInstace().getCurrentAvaiableCamera();
    auto& l_objectManager = p_scene.getObjectManager();
    p_scene.getWindow().poolEvents();
    glClearColor(p_scene.m_backgroundColor.x, 
                 p_scene.m_backgroundColor.y, 
                 p_scene.m_backgroundColor.z, 
                 p_scene.m_backgroundColor.w);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto& object : l_objectManager.getObjects())
    {
        const int WIDTH = 1270;
        const int HEIGHT = 720;
        auto& l_shaderProgram = object.first.m_shaderProgram;

        l_shaderProgram->bindShaderProgram();

        l_objectManager.processObject(object.first);

        glm::mat4 l_projection;
        l_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

        l_camera->rotateCamera();
        l_camera->moveCamera();

        l_shaderProgram->uniformVec3(l_camera->getCameraPosition(), "cameraPos");
        l_shaderProgram->uniformMatrix4(l_camera->getViewMatrix(), "view");
        l_shaderProgram->uniformMatrix4(l_projection, "projection");

        for(auto& mesh : object.first.m_meshes)
        {
            Textures::TextureManager::getInstance().activeTexturesForCustomObject(*mesh, *l_shaderProgram);
            mesh->m_vertexArray.bindVao();
            glDrawElements(GL_TRIANGLES, mesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
            mesh->m_vertexArray.unbindVao();
        }

        l_shaderProgram->unbindShaderProgram();
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    p_scene.getWindow().swapBuffer();
}

RenderManager & RenderManager::getInstance()
{
    static RenderManager renderManager;
    return renderManager;
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