#pragma once
#include "Window.h"
#include "Scene.h"
#include "ScreenQuadBuilder.h"

namespace McEngine
{
namespace GameWindow
{
class WindowManager
{
public:
    
    void start();

    void shutdown();

    void prepareWindow(const std::string & p_title, int p_width, int p_height);

    void setCurrentScene(std::shared_ptr<Scenes::Scene>& p_scene);

    glm::vec4 getBackgroundColor() const;

    void createFrameBuffers();

    void bindEditorFrameBuffer();

    void unbindFrameBuffer();

    void bindGameFrameBuffer();

    void buildFrameBuffer(bool p_isEditorFrameBuffer, GLuint & p_colorTextureId);

    void createWindowPlane();

    void setScreenUniform();

    void activeEditorQuad();

    void activeGameQuad();

    void deactiveEditorQuad();

    void deactiveGameQuad();

    bool checkIfFrameBufferCorrectlyBuild();

    void createColorAttachment(GLuint & p_textureColorId);

    void createRenderBufferObject();

    void updateViewPort();

    Window& getWindow();

    static WindowManager& getInstance();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:    
    WindowManager() = default;

    void activeQuad(GLuint p_colorTextureId);
    void deactiveQuad();

    Window m_window;
    glm::vec4 m_backgroundColor;
    std::shared_ptr<Scenes::Scene> m_currentScene;
    GLuint m_editorWindowFrameBuffer;
    GLuint m_gameWindowFrameBuffer;
    GLuint m_renderBufferObject;

    GLuint m_editorColorTextureId;
    GLuint m_gameColorTextureId;

    Meshes::Object m_windowPlane;
    bool m_editMode;
};

}//GameWindow
}//McEngine

