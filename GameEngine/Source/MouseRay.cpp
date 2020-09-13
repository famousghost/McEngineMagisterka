#include "MouseRay.h"
#include "Geometry3dUtils.h"
#include <iostream>

namespace McEngine
{
namespace Inputs
{

MouseRay::MouseRay()
{
}


MouseRay::~MouseRay()
{
}

glm::vec3 MouseRay::getMousePosition()
{
    auto* l_currentWindow = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();

    double l_mouseXPosition;
    double l_mouseYPosition;
    glfwGetCursorPos(l_currentWindow, &l_mouseXPosition, &l_mouseYPosition);
    glm::vec2 l_ndc = convertToNDC(l_mouseXPosition, l_mouseYPosition);
    glm::vec4 l_clipSpace = glm::vec4(l_ndc.x, l_ndc.y, -1.0f, 1.0f);
    glm::vec4 l_viewSpace = convertToViewSpace(l_clipSpace);
    glm::vec3 l_worldSpace = convertToWorldSpace(l_viewSpace);
    return l_worldSpace;
}

bool MouseRay::checkIntersectionWithCube(const Meshes::Object& p_object)
{
    Meshes::Ray l_ray(Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getEditorCamera()->getCameraPosition(), 
                      getMousePosition());
    if (Utils::Geometry3dUtils::raycast(p_object, l_ray))
    {
        return true;
    }
    return false;
}

glm::vec3 MouseRay::convertToWorldSpace(const glm::vec4 p_viewSpace)
{
    auto& l_currentCamera = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getEditorCamera();
    glm::vec4 l_worldSpaceIn4D = glm::inverse(l_currentCamera->getViewMatrix()) * p_viewSpace;
    glm::vec3 l_worldSpace = glm::vec3(l_worldSpaceIn4D.x, l_worldSpaceIn4D.y, l_worldSpaceIn4D.z);
    return glm::normalize(l_worldSpace);
}

glm::vec4 MouseRay::convertToViewSpace(const glm::vec4& p_clipSpace)
{
    auto& l_currentCamera = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getEditorCamera();
    glm::vec4 l_viewSpace = glm::inverse(l_currentCamera->getProjectionMatrix()) * p_clipSpace;
    l_viewSpace = glm::vec4(l_viewSpace.x, l_viewSpace.y, -1.0f, 0.0f);
    return glm::normalize(l_viewSpace);
}

glm::vec2 MouseRay::convertToNDC(double p_mouseXPosition, double p_mouseYPosition)
{
    auto* l_currentWindow = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    int l_width;
    int l_height;
    glfwGetWindowSize(l_currentWindow, &l_width, &l_height);
    float x = (2.0f * p_mouseXPosition) / (l_width/2) - 1.0f;
    float y = 1.0f - (2.0f * p_mouseYPosition) / (l_height/2);
    return glm::vec2(x, y);
}

}//Inputs
}//McEngine