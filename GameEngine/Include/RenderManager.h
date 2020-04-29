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

    static RenderManager& getInstance();
private:
    RenderManager() = default;
    ~RenderManager() = default;

    void setProjectionMatrix(float p_fov, 
                             float p_near, 
                             float p_far, 
                             Shaders::Shader& p_shaderProgram);

    void drawMeshes(Meshes::Object& p_object);

    void drawObjects(Scenes::Scene& p_scene);

    void initGlad();
};

}//Renderer
}//McEngine
