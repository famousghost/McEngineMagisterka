#include "Camera.h"

namespace McEngine
{
namespace Cameras
{

Camera::Camera()
{
    m_cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{
    
}

void Camera::moveCamera()
{
    
}

void Camera::rotateCamera()
{
    
}

}//Cameras
}//McEngine