#pragma once
#include "Scene.h"

namespace McEngine
{
namespace Renderer
{

class RenderManager
{
public:
    void start();

    void shutdown();
    void drawSkybox(Meshes::ObjectManager & p_objectManager, Cameras::Camera & p_camera);
    void draw(Scenes::Scene& p_scene);

    void drawToCubeMap(Scenes::Scene & p_scene, std::shared_ptr<Cameras::Camera>& p_camera);

    void drawCompleteScene(Scenes::Scene & p_scene, std::shared_ptr<Cameras::Camera>& p_camera);

    void showMesh();

    void fillMesh();

    void drawColliders(Meshes::Object & p_object, Cameras::Camera & p_camera, Meshes::ObjectManager & p_objectManager);

    static RenderManager& getInstance();
private:
    RenderManager() = default;
    ~RenderManager() = default;

    void drawMeshes(Meshes::Object& p_object);

    void drawObjects(Scenes::Scene & p_scene, std::shared_ptr<Cameras::Camera>& p_camera);

    void drawEditorWindow();

    void drawGameWindow();

    void drawScene();

    void initGlad();

    bool m_fillMesh;

    GLuint m_generatedCubeMapTexture;
    GLuint m_cubeMapFrameBuffer;
    GLuint m_cubeMapRenderBuffer;
};

}//Renderer
}//McEngine
