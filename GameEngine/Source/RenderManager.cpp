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

namespace
{
    const glm::vec2 l_cameraDir[6]
    {
       //    "Textures/Cubemap/xpos.jpg",
        //"Textures/Cubemap/xneg.jpg",
       // "Textures/Cubemap/ypos.jpg",
       // "Textures/Cubemap/yneg.jpg",
       // "Textures/Cubemap/zpos.jpg",
       // "Textures/Cubemap/zneg.jpg"
        glm::vec2(0.0f, 0.0f), // Right
        glm::vec2(-180.0f, 0.0f), // Left
        glm::vec2(-90.0f, 89.0f), // Up
        glm::vec2(-90.0f, -89.0f), // Down
        glm::vec2(90.0f, 0.0f), // Back
        glm::vec2(-90.0f, 0.0f) //Front
    };
}

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

    //drawToCubeMap(p_scene, l_editorCamera);

    l_windowManager.updateViewPort();

    l_windowManager.bindEditorFrameBuffer();

    drawCompleteScene(p_scene, l_editorCamera);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    l_windowManager.unbindFrameBuffer();

    l_windowManager.bindGameFrameBuffer();

    drawCompleteScene(p_scene, l_gameCamera);

    l_windowManager.unbindFrameBuffer();

    drawScene();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    l_window.swapBuffer();
}

void RenderManager::drawToCubeMap(Scenes::Scene& p_scene, std::shared_ptr<Cameras::Camera>& p_camera)
{
    auto& l_objectManager = p_scene.getObjectManager();
    auto& l_window = GameWindow::WindowManager::getInstance().getWindow();

    glGenTextures(1, &m_generatedCubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_generatedCubeMapTexture);

    int l_width;
    int l_height;
    glfwGetFramebufferSize(l_window.getGlfwWindow(), &l_width, &l_height);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<GLubyte> testData(l_width * l_height * 256, 128);
    std::vector<GLubyte> xData(l_width * l_height * 256, 255);

    for (int i = 0; i < 6; ++i)
    {
        //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, l_width, l_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        if (i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
                l_width, l_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &testData[0]);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
                l_width, l_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &xData[0]);
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glGenFramebuffers(1, &m_cubeMapFrameBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFrameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_generatedCubeMapTexture, 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenRenderbuffers(1, &m_cubeMapRenderBuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, m_cubeMapRenderBuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, l_width, l_height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_cubeMapRenderBuffer);

    glViewport(0, 0, l_width, l_height);
    int i = 0;
    for (auto dir : l_cameraDir)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        p_camera->updateCameraAngle(dir.x, dir.y);

        drawSkybox(l_objectManager, *p_camera);

        drawObjects(p_scene, p_camera);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_generatedCubeMapTexture, 0);

        i++;
    }
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG("ERROR::FRAMEBUFFER::Framebuffer is not complete I DONT KNOW WHY!", LogType::ERR);
        //exit(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteRenderbuffers(1, &m_cubeMapRenderBuffer);
    glDeleteFramebuffers(1, &m_cubeMapFrameBuffer);
}

void RenderManager::drawCompleteScene(Scenes::Scene& p_scene, std::shared_ptr<Cameras::Camera>& p_camera)
{
    auto& l_objectManager = p_scene.getObjectManager();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p_camera->update();

    drawSkybox(l_objectManager, *p_camera);

    drawObjects(p_scene, p_camera);
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
        if (l_object.m_currentActiveShader == "reflectionEnv")
        {
            continue;
        }
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
        l_object.m_shaderProgram->uniform1I(m_generatedCubeMapTexture, "envMap");
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