#pragma once
#include "Window.h"
#include "Scene.h"
#include "ScreenQuadBuilder.h"

namespace McEngine
{
namespace GameWindow
{
enum class FrameBufferType
{
    EDITOR = 0,
    GAME
};
class WindowManager
{
public:
    
    void start();

    void shutdown();

    void prepareWindow(const std::string & p_title, int p_width, int p_height);

    void setCurrentScene(std::shared_ptr<Scenes::Scene>& p_scene);

    glm::vec4 getBackgroundColor() const;

    void setBackgroundColor(glm::vec4 p_backgroundColor);

    void createFrameBuffers();

    void bindEditorFrameBuffer();

    void unbindFrameBuffer();

    void bindGameFrameBuffer();

    void bindShadowFrameBuffer();

    void buildShadowMapFrameBuffer(GLuint & p_depthMap);

    void prepareDepthTexture(int p_shadowWidth, int p_shadowHeight, GLuint & p_depthMap);

    void buildFrameBuffer(FrameBufferType p_isEditorFrameBuffer, GLuint & p_colorTextureId);

    void createWindowPlane();

    void setScreenUniform();

    void activeEditorQuad();

    void activeGameQuad();

    void activeShadowMapQuad();

    void deactiveQuad();

    bool checkIfFrameBufferCorrectlyBuild();

    void createColorAttachment(GLuint & p_textureColorId);

    void createRenderBufferObject();

    void bindShadowMap();

    void updateViewPort();

    GLuint getShadowMapId() const;

    void updateShadowMapViewPort();

    Window& getWindow();

    static WindowManager& getInstance();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:    
    WindowManager() = default;

    void activeQuad(GLuint p_colorTextureId);

    Window m_window;
    glm::vec4 m_backgroundColor;
    std::shared_ptr<Scenes::Scene> m_currentScene;
    GLuint m_editorWindowFrameBuffer;
    GLuint m_gameWindowFrameBuffer;
    GLuint m_renderBufferObject;
    GLuint m_shadowFrameBuffer;

    GLuint m_editorColorTextureId;
    GLuint m_gameColorTextureId;
    GLuint m_shadowMapTextureId;

    Meshes::Object m_windowPlane;
    bool m_editMode;
};

}//GameWindow
}//McEngine

