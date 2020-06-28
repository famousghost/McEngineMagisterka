#include "Camera.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include <math.h>
#include "Logger.h"

namespace McEngine
{
namespace Cameras
{

namespace
{
    constexpr float SPEED_AXIS_Z = 50.0f;
    constexpr float MOVEMENT_SPEED = 5.0f;
    constexpr float SPEED_AXIS_X_Y = 5.0f;
    constexpr float SENSITIVITY = 40.0f;
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
    float l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();

    m_cameraPosition += m_cameraFront * l_inputManager.s_cameraMoveSpeedOnAxisZ * SPEED_AXIS_Z * l_deltaTime;
    glm::vec3 l_newCameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    glm::vec3 l_newCameraUp = glm::normalize(glm::cross(l_newCameraRight, m_cameraFront));
    m_cameraPosition += l_newCameraRight * l_inputManager.s_cameraMoveSpeedOnAxisX * SPEED_AXIS_X_Y * l_deltaTime;
    m_cameraPosition += l_newCameraUp * l_inputManager.s_cameraMoveSpeedOnAxisY * SPEED_AXIS_X_Y * l_deltaTime;
    l_inputManager.s_cameraMoveSpeedOnAxisZ = 0.0f;
    l_inputManager.s_cameraMoveSpeedOnAxisX = 0.0f;
    l_inputManager.s_cameraMoveSpeedOnAxisY = 0.0f;

    m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

void Camera::moveGameCamera()
{
    auto& l_inputManager = Inputs::InputManager::getInstance();
    float l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();

    if(l_inputManager.getKeyDown(GLFW_KEY_W))
    {
        m_cameraPosition += m_cameraFront * MOVEMENT_SPEED * l_deltaTime;
    }
    if (l_inputManager.getKeyDown(GLFW_KEY_S))
    {
        m_cameraPosition -= m_cameraFront * MOVEMENT_SPEED * l_deltaTime;
    }

    glm::vec3 l_newCameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    if(l_inputManager.getKeyDown(GLFW_KEY_A))
    {
        m_cameraPosition -= l_newCameraRight * MOVEMENT_SPEED * l_deltaTime;
    }
    if (l_inputManager.getKeyDown(GLFW_KEY_D))
    {
        m_cameraPosition += l_newCameraRight * MOVEMENT_SPEED * l_deltaTime;
    }
    m_view = glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

void Camera::rotateCamera()
{
    auto& l_inputManager = Inputs::InputManager::getInstance();

    float l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();
    m_yaw += l_inputManager.s_cameraRotateSpeedOnAxisX * SENSITIVITY * l_deltaTime;
    m_pitch += l_inputManager.s_cameraRotateSpeedOnAxisY * SENSITIVITY * l_deltaTime;

    l_inputManager.s_cameraRotateSpeedOnAxisY = 0.0f;
    l_inputManager.s_cameraRotateSpeedOnAxisX = 0.0f;

    if (m_pitch >= 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch <= -89.0f)
    {
        m_pitch = -89.0f;
    }

    glm::vec3 l_newFront;
    l_newFront.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
    l_newFront.y = std::sin(glm::radians(m_pitch));
    l_newFront.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));

    m_cameraFront = glm::normalize(l_newFront);
}

void Camera::setProjectionMatrix(float p_fov,
                                 float p_near,
                                 float p_far,
                                 Shaders::Shader& p_shaderProgram,
                                 const std::string& p_projectionMatrixUniform)
{
    auto* l_window = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    int l_width;
    int l_height;
    glfwGetWindowSize(l_window, &l_width, &l_height);

    m_projectionMatrix = glm::perspective(glm::radians(p_fov),
                                          static_cast<float>(l_width) / static_cast<float>(l_height),
                                          p_near,
                                          p_far);

    p_shaderProgram.uniformMatrix4(m_projectionMatrix, p_projectionMatrixUniform);
}

void Camera::update()
{
    rotateCamera();
    moveCamera();
}

void Camera::updateGameCamera()
{
    moveGameCamera();
}

void Camera::updateShaderProgram(Shaders::Shader& p_shaderProgram,
                                 const std::string& p_cameraPostionUniform,
                                 const std::string& p_viewMatrixUniform,
                                 const std::string& p_projectionMatrixUniform)
{
    p_shaderProgram.uniformVec3(m_cameraPosition, p_cameraPostionUniform);
    p_shaderProgram.uniformMatrix4(m_view, p_viewMatrixUniform);
    setProjectionMatrix(45.0f, 0.1f, 100.0f, p_shaderProgram, p_projectionMatrixUniform);
}

void Camera::updateShaderProgramForSkybox(Shaders::Shader& p_shaderProgram,
                                          const std::string& p_cameraPostionUniform,
                                          const std::string& p_viewMatrixUniform,
                                          const std::string& p_projectionMatrixUniform)
{
    p_shaderProgram.uniformVec3(m_cameraPosition, p_cameraPostionUniform);
    glm::mat4 view = glm::mat4(glm::mat3(m_view));
    p_shaderProgram.uniformMatrix4(view, p_viewMatrixUniform);
    setProjectionMatrix(45.0f, 0.1f, 100.0f, p_shaderProgram, p_projectionMatrixUniform);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return m_projectionMatrix;
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