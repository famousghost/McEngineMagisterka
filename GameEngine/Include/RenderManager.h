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
    void draw(Scenes::Scene& p_scene);

    void showMesh();

    void fillMesh();

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
};

}//Renderer
}//McEngine
