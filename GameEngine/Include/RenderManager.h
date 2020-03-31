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

    void initGlad();
};

}//Renderer
}//McEngine
