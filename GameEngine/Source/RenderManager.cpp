#include "RenderManager.h"
#include "ScenesManager.h"
#include "GuiManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "PhysicsManager.h"
#include "MouseRay.h"
#include "InputManager.h"

namespace McEngine
{
namespace Renderer
{

void RenderManager::start()
{
    m_fillMesh = true;
}

void RenderManager::shutdown()
{
    
}

void RenderManager::draw(Scenes::Scene & p_scene)
{
    auto& l_currentAvaiableScene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    auto& l_editorCamera = l_currentAvaiableScene->getEditorCamera();
    auto& l_gameCamera = l_currentAvaiableScene->getGameMainCamera();
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    auto& l_objectManager = p_scene.getObjectManager();
    auto& l_window = l_windowManager.getWindow();

    
    l_windowManager.updateViewPort();
    l_window.poolEvents();

    l_windowManager.bindEditorFrameBuffer();
    glClearColor(l_windowManager.getBackgroundColor().x,
                 l_windowManager.getBackgroundColor().y,
                 l_windowManager.getBackgroundColor().z,
                 l_windowManager.getBackgroundColor().w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObjects(p_scene, l_editorCamera);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    l_windowManager.unbindFrameBuffer();

    l_windowManager.bindGameFrameBuffer();
    glClearColor(l_windowManager.getBackgroundColor().x,
                 l_windowManager.getBackgroundColor().y,
                 l_windowManager.getBackgroundColor().z,
                 l_windowManager.getBackgroundColor().w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    drawObjects(p_scene, l_gameCamera);

    l_windowManager.unbindFrameBuffer();

    drawScene();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    l_window.swapBuffer();
}

void RenderManager::drawScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    drawEditorWindow();
    drawGameWindow();

    if (not m_fillMesh)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glEnable(GL_DEPTH_TEST);
}

void RenderManager::drawEditorWindow()
{
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    l_windowManager.activeEditorQuad();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    l_windowManager.deactiveEditorQuad();
}

void RenderManager::drawGameWindow()
{
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    l_windowManager.activeGameQuad();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    l_windowManager.deactiveGameQuad();
}

void RenderManager::showMesh()
{
    m_fillMesh = false;
}

void RenderManager::fillMesh()
{
    m_fillMesh = true;
}

void RenderManager::drawColliders(Meshes::Object& p_object,
                                  Cameras::Camera& p_camera,
                                  Meshes::ObjectManager& p_objectManager)
{
    auto& l_object = p_object;
    auto& l_collider = l_object.m_colider;
    l_collider.m_shaderProgram->bindShaderProgram();
    p_objectManager.updateCollider(p_object, p_camera);
    if (Gui::GuiManager::getInstance().getColliderVisiblity())
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        auto& l_coliderMesh = l_object.m_colider.m_meshes.at(0);
        l_coliderMesh->m_vertexArray.bindVao();
        glDrawElements(GL_TRIANGLES, l_coliderMesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
        l_coliderMesh->m_vertexArray.unbindVao();

        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    l_collider.m_shaderProgram->unbindShaderProgram();
}

void RenderManager::drawObjects(Scenes::Scene & p_scene, std::shared_ptr<Cameras::Camera>& p_camera)
{
    auto& l_objectManager = p_scene.getObjectManager();

    auto& l_objects = l_objectManager.getObjects();
    auto& l_physicsManager = Physics::PhysicsManager::getInstance();

    for (auto& object : l_objects)
    {
        auto& l_object = object.first;
        auto& l_shaderProgram = *l_object.m_shaderProgram;

        drawColliders(l_object, *p_camera, l_objectManager);

        l_shaderProgram.bindShaderProgram();

        l_objectManager.update(l_object);
        p_camera->update(l_shaderProgram, "cameraPos", "view", "projection");

        drawMeshes(l_object);

        l_shaderProgram.unbindShaderProgram();

        if (m_fillMesh)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (Inputs::InputManager::getInstance().s_onClickMouse)
        {
            auto& l_colider = l_object.m_colider;
        
            Inputs::MouseRay l_mouseRay;

            if (l_mouseRay.checkIntersectionWithCube(glm::vec3(l_colider.m_minVertex), 
                                                     glm::vec3(l_colider.m_maxVertex)))
            {
                Gui::GuiManager::getInstance().chooseObjectViaMouse(l_object.m_objectName);
                l_object.m_material.m_highlightColor = glm::vec3(0.0f, 1.0f, 0.0f);
            }
            else
            {
                l_object.m_material.m_highlightColor = l_object.m_material.m_objectColor;
            }
        }
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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