#include "Camera.h"
#include "InputManager.h"
#include <math.h>
#include "Logger.h"

namespace McEngine
{
namespace Cameras
{

namespace
{
    constexpr float SPEED = 0.01f;
    constexpr float SENSITIVITY = 0.001f;
}

Camera::Camera()
{
    m_cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    m_pitch = 0.0f;
    m_yaw = -90.0f;
}

Camera::~Camera()
{
    
}

void Camera::moveCamera()
{
    auto& l_inputManager = Inputs::InputManager::getInstance();

    m_cameraPosition += l_inputManager.s_cameraMoveSpeedOnAxisZ * m_cameraFront;
    glm::vec3 l_newCameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    glm::vec3 l_newCameraUp = glm::normalize(glm::cross(l_newCameraRight, m_cameraFront));
    m_cameraPosition += l_newCameraRight * l_inputManager.s_cameraMoveSpeedOnAxisX * SPEED;
    m_cameraPosition += l_newCameraUp * l_inputManager.s_cameraMoveSpeedOnAxisY * SPEED;
    l_inputManager.s_cameraMoveSpeedOnAxisZ = 0.0f;
    l_inputManager.s_cameraMoveSpeedOnAxisX = 0.0f;
    l_inputManager.s_cameraMoveSpeedOnAxisY = 0.0f;

    m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

void Camera::rotateCamera()
{
    auto& l_inputManager = Inputs::InputManager::getInstance();

    m_yaw += l_inputManager.s_cameraRotateSpeedOnAxisX * SENSITIVITY;
    m_pitch += l_inputManager.s_cameraRotateSpeedOnAxisY * SENSITIVITY;

    l_inputManager.s_cameraRotateSpeedOnAxisY = 0.0f;
    l_inputManager.s_cameraRotateSpeedOnAxisX = 0.0f;

    glm::vec3 l_newFront;
    l_newFront.x = std::cos(m_pitch) * std::cos(m_yaw);
    l_newFront.y = std::sin(m_pitch);
    l_newFront.z = std::cos(m_pitch) * std::sin(m_yaw);

    m_cameraFront = l_newFront;
}

glm::vec3 Camera::getCameraPosition() const
{
    return m_cameraPosition;
}

glm::mat4 Camera::getViewMatrix() const
{
    return m_view;
}

}//Cameras
}//McEngine