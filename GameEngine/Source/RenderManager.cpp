#include "RenderManager.h"
#include "ScenesManager.h"
#include "GuiManager.h"
#include "TextureManager.h"
#include "MouseRay.h"

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

    drawObjects(p_scene);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    p_scene.getWindow().swapBuffer();
}

void RenderManager::drawObjects(Scenes::Scene & p_scene)
{
    auto& l_camera = Scenes::ScenesManager::getInstace().getCurrentAvaiableCamera();
    auto& l_objectManager = p_scene.getObjectManager();
    for (auto& object : l_objectManager.getObjects())
    {
        auto& l_object = object.first;
        auto& l_shaderProgram = *l_object.m_shaderProgram;

        l_shaderProgram.bindShaderProgram();

        l_objectManager.update(l_object);
        l_camera->update(l_shaderProgram, "cameraPos", "view", "projection");

        drawMeshes(l_object);

        l_shaderProgram.unbindShaderProgram();
    }
    Inputs::MouseRay l_mouseRay;

    auto l_mousePosInWorldSpace = l_mouseRay.getMousePosition();

    //Debug onlive
    /*std::cout << "x: " << l_mousePosInWorldSpace.x
              << " y: " << l_mousePosInWorldSpace.y
              << " z: " << l_mousePosInWorldSpace.z
              << std::endl;*/
}

void RenderManager::drawMeshes(Meshes::Object& p_object)
{
    for (auto& mesh : p_object.m_meshes)
    {
        Textures::TextureManager::getInstance().activeTexturesForCustomObject(*mesh, *p_object.m_shaderProgram);
        mesh->m_vertexArray.bindVao();
        glDrawElements(GL_TRIANGLES, mesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
        mesh->m_vertexArray.unbindVao();
    }
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