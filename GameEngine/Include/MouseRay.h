#pragma once
#include "ScenesManager.h"
namespace McEngine
{
namespace Inputs
{
class MouseRay
{
public:
    MouseRay();
    ~MouseRay();

    glm::vec3 getMousePosition();

private:
    glm::vec3 convertToWorldSpace(const glm::vec4 p_viewSpace);
    glm::vec4 convertToViewSpace(const glm::vec4 & p_clipSpace);
    glm::vec2 convertToNDC(double p_mouseXPosition, double p_mouseYPosition);
};
}//Inputs
}//McEngine
