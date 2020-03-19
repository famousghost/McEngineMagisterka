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
    const int WIDTH = 1270;
    const int HEIGHT = 720;
    auto& objects = p_scene.getObjectManager().getObjects();
    p_scene.getWindow().poolEvents();
    glClearColor(p_scene.m_backgroundColor.x, 
                 p_scene.m_backgroundColor.y, 
                 p_scene.m_backgroundColor.z, 
                 p_scene.m_backgroundColor.w);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 l_view;
    l_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 l_projection;
    l_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);


    for(auto& object : objects)
    {
        object.first.m_vertexArray->bindVao();
        object.first.m_shaderProgram->bindShaderProgram();

        glm::mat4 l_model;
        l_model = glm::translate(l_model, object.first.m_position);
        l_model = glm::rotate(l_model, glm::radians(object.first.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
        l_model = glm::rotate(l_model, glm::radians(object.first.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
        l_model = glm::rotate(l_model, glm::radians(object.first.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
        l_model = glm::scale(l_model, object.first.m_scale);

        object.first.m_shaderProgram->uniformMatrix4(l_model, "model");
        object.first.m_shaderProgram->uniformMatrix4(l_view, "view");
        object.first.m_shaderProgram->uniformMatrix4(l_projection, "projection");

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