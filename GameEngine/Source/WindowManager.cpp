#include "WindowManager.h"
#include "ScenesManager.h"
#include "Logger.h"

namespace McEngine
{
namespace GameWindow
{

namespace
{
    constexpr float GAMESCREEN_OFFSET = 1.0f;
    const char * SCREEN_OFFSET_UNIFORM = "screenPositionOffset";
}

void WindowManager::start()
{
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    prepareWindow("McEngine", mode->width, mode->height);
    m_editMode = true;
    m_window.setCurrentContext();
    glfwSetFramebufferSizeCallback(m_window.getGlfwWindow(), framebuffer_size_callback);
    auto& l_currentScene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    setCurrentScene(l_currentScene);
    if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("Failed to initialize GLAD", LogType::ERR);
        exit(0);
    }
    createFrameBuffers();
    buildFrameBuffer(FrameBufferType::EDITOR, m_editorColorTextureId);
    buildFrameBuffer(FrameBufferType::GAME, m_gameColorTextureId);
    buildShadowMapFrameBuffer(m_shadowMapTextureId);

}

void WindowManager::shutdown()
{
    glDeleteBuffers(1, &m_editorWindowFrameBuffer);
    glDeleteBuffers(1, &m_gameWindowFrameBuffer);
}

void WindowManager::prepareWindow(const std::string& p_title, int p_width, int p_height)
{
    m_backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    m_window.createWindow(p_title.c_str(), p_width, p_height);
}

void WindowManager::setCurrentScene(std::shared_ptr<Scenes::Scene>& p_scene)
{
    m_currentScene = p_scene;
}

glm::vec4 WindowManager::getBackgroundColor() const
{
    return m_backgroundColor;
}

void WindowManager::setBackgroundColor(glm::vec4 p_backgroundColor)
{
    m_backgroundColor = p_backgroundColor;
}

void WindowManager::createFrameBuffers()
{
    glGenFramebuffers(1, &m_editorWindowFrameBuffer);
    glGenFramebuffers(1, &m_gameWindowFrameBuffer);
    glGenFramebuffers(1, &m_shadowFrameBuffer);
}

void WindowManager::bindEditorFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_editorWindowFrameBuffer);
}

void WindowManager::unbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WindowManager::bindGameFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gameWindowFrameBuffer);
}

void WindowManager::bindShadowFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFrameBuffer);
}

void WindowManager::buildShadowMapFrameBuffer(GLuint& p_depthMap)
{
    int l_shadowWidth = 1024;
    int l_shadowHeight = 1024;

    prepareDepthTexture(l_shadowWidth, l_shadowHeight, p_depthMap);

    bindShadowFrameBuffer();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (checkIfFrameBufferCorrectlyBuild())
    {
        LOG("ERROR::FRAMEBUFFER:: shadow mpa is not complete!", LogType::ERR);
        exit(0);
    }

    unbindFrameBuffer();
}

void WindowManager::prepareDepthTexture(int p_shadowWidth, int p_shadowHeight, GLuint& p_depthMap)
{
    glGenTextures(1, &p_depthMap);
    glBindTexture(GL_TEXTURE_2D, p_depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        p_shadowWidth, p_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float l_borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, l_borderColor);
}

void WindowManager::buildFrameBuffer(FrameBufferType p_frameBufferType, GLuint& p_colorTextureId)
{

    int l_width;
    int l_height;
    switch (p_frameBufferType)
    {
    case McEngine::GameWindow::FrameBufferType::EDITOR:
        bindEditorFrameBuffer();
        break;
    case McEngine::GameWindow::FrameBufferType::GAME:
        bindGameFrameBuffer();
        break;
    default:
        LOG("CANNOT FIND FRAMEBUFFER TYPE", LogType::ERR);
        break;
    }
    createColorAttachment(p_colorTextureId);
    createRenderBufferObject();
    if (checkIfFrameBufferCorrectlyBuild())
    {
        LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!", LogType::ERR);
        exit(0);
    }
    unbindFrameBuffer();
}

void WindowManager::createWindowPlane()
{
    Meshes::ScreenQuadBuilder l_planeBuilder;
    l_planeBuilder.addMesh().addShaderProgram("windowShader");
    m_windowPlane = l_planeBuilder.getObject();
    setScreenUniform();
}

void WindowManager::setScreenUniform()
{
    m_windowPlane.m_shaderProgram->bindShaderProgram();
    m_windowPlane.m_shaderProgram->uniform1I(0, "screenTexture");
    m_windowPlane.m_shaderProgram->unbindShaderProgram();
}

void WindowManager::activeEditorQuad()
{
    m_windowPlane.m_shaderProgram->bindShaderProgram();
    m_windowPlane.m_shaderProgram->uniform1f(0.0f, SCREEN_OFFSET_UNIFORM);
    m_windowPlane.m_shaderProgram->unbindShaderProgram();
    activeQuad(m_editorColorTextureId);
}

void WindowManager::activeGameQuad()
{
    m_windowPlane.m_shaderProgram->bindShaderProgram();
    m_windowPlane.m_shaderProgram->uniform1f(GAMESCREEN_OFFSET, SCREEN_OFFSET_UNIFORM);
    m_windowPlane.m_shaderProgram->unbindShaderProgram();
    activeQuad(m_gameColorTextureId);
}

void WindowManager::activeShadowMapQuad()
{
    m_windowPlane.m_shaderProgram->bindShaderProgram();
    m_windowPlane.m_shaderProgram->uniform1f(GAMESCREEN_OFFSET, SCREEN_OFFSET_UNIFORM);
    m_windowPlane.m_shaderProgram->uniform1f(1.0f, "near_plane");
    m_windowPlane.m_shaderProgram->uniform1f(7.5f, "far_plane");
    m_windowPlane.m_shaderProgram->unbindShaderProgram();
    activeQuad(m_shadowMapTextureId);
}

bool WindowManager::checkIfFrameBufferCorrectlyBuild()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
}

void WindowManager::createColorAttachment(GLuint& p_textureColorId)
{
    int l_width;
    int l_height;
    glfwGetFramebufferSize(m_window.getGlfwWindow(), &l_width, &l_height);
    glGenTextures(1, &p_textureColorId);
    glBindTexture(GL_TEXTURE_2D, p_textureColorId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_width, l_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p_textureColorId, 0);
}

void WindowManager::createRenderBufferObject()
{
    int l_width;
    int l_height;
    glfwGetFramebufferSize(m_window.getGlfwWindow(), &l_width, &l_height);
    glGenRenderbuffers(1, &m_renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, l_width, l_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject);
}

void WindowManager::updateViewPort()
{
    int l_width;
    int l_height;
    glfwGetFramebufferSize(m_window.getGlfwWindow(), &l_width, &l_height);
    glViewport(0, 0, l_width, l_height);
}

GLuint WindowManager::getShadowMapId() const
{
    return m_shadowMapTextureId;
}

void WindowManager::updateShadowMapViewPort()
{
    glViewport(0, 0, 1024, 1024);
}

Window& WindowManager::getWindow()
{
    return m_window;
}

WindowManager & WindowManager::getInstance()
{
   static WindowManager windowManager;
   return windowManager;
}

void WindowManager::activeQuad(GLuint p_colorTextureId)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, p_colorTextureId);
    m_windowPlane.m_shaderProgram->bindShaderProgram();
    m_windowPlane.m_meshes.at(0)->m_vertexArray.bindVao();
}

void WindowManager::deactiveQuad()
{
    m_windowPlane.m_meshes.at(0)->m_vertexArray.unbindVao();
    m_windowPlane.m_shaderProgram->unbindShaderProgram();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void WindowManager::bindShadowMap()
{
    glBindTexture(GL_TEXTURE_2D, m_shadowMapTextureId);
}

void WindowManager::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

}//GameWindow
}//McEngine