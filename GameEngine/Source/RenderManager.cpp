#include "RenderManager.h"
#include "ScenesManager.h"
#include "GuiManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "PhysicsManager.h"
#include "MouseRay.h"
#include "InputManager.h"
#include "ShaderManager.h"

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

void RenderManager::drawSkybox(Meshes::ObjectManager& p_objectManager,
                               Cameras::Camera& p_camera)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_FALSE);

    auto& l_skybox = p_objectManager.getSkybox();
    auto& l_shaderProgram = l_skybox.m_shaderProgram;
    auto& l_mesh = l_skybox.m_meshes.at(0);
    l_shaderProgram->bindShaderProgram();
    Textures::TextureManager::getInstance().activeCubemapTexture(l_skybox);
    p_camera.updateShaderProgramForSkybox(*l_shaderProgram, "cameraPos", "view", "projection");
    l_mesh->m_vertexArray.bindVao();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    l_mesh->m_vertexArray.unbindVao();
    l_shaderProgram->unbindShaderProgram();

    glDepthMask(GL_TRUE);
}

void RenderManager::draw(Scenes::Scene & p_scene)
{
    auto& l_currentAvaiableScene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    auto& l_editorCamera = l_currentAvaiableScene->getEditorCamera();
    auto& l_gameCamera = l_currentAvaiableScene->getGameMainCamera();
    auto& l_shadowMapCamera = l_currentAvaiableScene->getShadowMapCamera();
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    auto& l_objectManager = p_scene.getObjectManager();
    auto& l_window = l_windowManager.getWindow();
    l_editorCamera->setEditorScene(true);
    l_gameCamera->setEditorScene(false);
    l_shadowMapCamera->setEditorScene(false);
    
    l_window.poolEvents();

    glClearColor(l_windowManager.getBackgroundColor().x,
        l_windowManager.getBackgroundColor().y,
        l_windowManager.getBackgroundColor().z,
        l_windowManager.getBackgroundColor().w);

    l_windowManager.updateShadowMapViewPort();

    l_windowManager.bindShadowFrameBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    drawObjects(p_scene, l_shadowMapCamera);
    glCullFace(GL_BACK);

    l_windowManager.unbindFrameBuffer();

    l_windowManager.updateViewPort();

    l_windowManager.bindEditorFrameBuffer();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    l_editorCamera->update();

    drawSkybox(l_objectManager, *l_editorCamera);

    drawObjects(p_scene, l_editorCamera);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    l_windowManager.unbindFrameBuffer();

    l_windowManager.bindGameFrameBuffer();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    l_gameCamera->updateGameCamera();

    drawSkybox(l_objectManager, *l_gameCamera);

    drawObjects(p_scene, l_gameCamera);

    l_windowManager.unbindFrameBuffer();

    drawScene();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    l_window.swapBuffer();
}

void RenderManager::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawEditorWindow();
    drawGameWindow();
}

void RenderManager::drawEditorWindow()
{
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    l_windowManager.activeEditorQuad();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    l_windowManager.deactiveQuad();
}

void RenderManager::drawGameWindow()
{
    auto& l_windowManager = GameWindow::WindowManager::getInstance();
    l_windowManager.activeGameQuad();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    l_windowManager.deactiveQuad();
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
    if(p_object.m_colider.size() == 0)
    {
        return;
    }
    for(auto& collider : p_object.m_colider)
    {
        auto& l_object = p_object;
        collider.m_shaderProgram->bindShaderProgram();
        p_objectManager.updateCollider(p_object, p_camera);
        if (Gui::GuiManager::getInstance().getColliderVisiblity())
        {
            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            auto& l_coliderMesh = collider.m_meshes.at(0);
            l_coliderMesh->m_vertexArray.bindVao();
            glDrawElements(GL_TRIANGLES, l_coliderMesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
            l_coliderMesh->m_vertexArray.unbindVao();

            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        collider.m_shaderProgram->unbindShaderProgram();
    }
}

void RenderManager::drawObjects(Scenes::Scene & p_scene, std::shared_ptr<Cameras::Camera>& p_camera)
{
    auto& l_objectManager = p_scene.getObjectManager();

    auto& l_objects = l_objectManager.getObjects();
    auto& l_physicsManager = Physics::PhysicsManager::getInstance();
    auto l_near = l_objectManager.l_nearFarPlane.x;
    auto l_far = l_objectManager.l_nearFarPlane.y;
    for (auto& object : l_objects)
    {
        auto& l_object = object.first;
        auto l_prevShaderProgram = l_object.m_shaderProgram;
        auto& l_currentShaderProgram = l_object.m_shaderProgram;
        p_camera->setLightPosition(l_objectManager.m_lightPosition);
        if (p_camera->isShadowMappnig())
        {
            l_currentShaderProgram = Shaders::ShaderManager::getInstance().getShader("shadowMapShader");
            l_currentShaderProgram->bindShaderProgram();

            l_objectManager.update(l_object);
            p_camera->updateShaderProgramForShadowMapping(l_near, l_far, *l_currentShaderProgram);

            drawMeshes(l_object);

            l_currentShaderProgram->unbindShaderProgram();

            l_currentShaderProgram = l_prevShaderProgram;
            continue;
        }
        if (p_camera->isEditorScene())
        {
            if (m_fillMesh)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }

        drawColliders(l_object, *p_camera, l_objectManager);

        l_currentShaderProgram->bindShaderProgram();

        l_objectManager.update(l_object);
        p_camera->updateShaderProgram(*l_currentShaderProgram, "cameraPos", "view", "projection");
        p_camera->updateShaderProgramForShadowMapping(l_near, l_far, *l_currentShaderProgram);

        drawMeshes(l_object);

        l_currentShaderProgram->unbindShaderProgram();

        if (Inputs::InputManager::getInstance().s_onClickMouse)
        {
            Inputs::MouseRay l_mouseRay;

            if (l_mouseRay.checkIntersectionWithCube(l_object))
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
        p_object.m_shaderProgram->uniform1I(10, "cubemap");
        Textures::TextureManager::getInstance().activeCubemapTexture(p_object);
        mesh->m_vertexArray.bindVao();
        glDrawElements(GL_TRIANGLES, mesh->m_indicies.size(), GL_UNSIGNED_INT, 0);
        mesh->m_vertexArray.unbindVao();
    }
}

RenderManager& RenderManager::getInstance()
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