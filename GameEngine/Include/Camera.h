#pragma once
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include "Shader.h"

namespace McEngine
{
namespace Cameras
{
class Camera
{
public:
    Camera();
    ~Camera();

    void moveCamera();

    void rotateCamera();

    void update();

    void updateShaderProgram(Shaders::Shader & p_shaderProgram, const std::string & p_cameraPostionUniform, const std::string & p_viewMatrixUniform, const std::string & p_projectionMatrixUniform);

    void updateShaderProgramForSkybox(Shaders::Shader & p_shaderProgram, const std::string & p_cameraPostionUniform, const std::string & p_viewMatrixUniform, const std::string & p_projectionMatrixUniform);

    glm::mat4 getProjectionMatrix() const;

    glm::vec3 getCameraPosition() const;

    glm::mat4 getViewMatrix() const;
private:
    void setProjectionMatrix(float p_fov,
                             float p_near,
                             float p_far,
                             Shaders::Shader & p_shaderProgram,
                             const std::string& p_projectionMatrixUniform);
    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::mat4 m_view;
    glm::mat4 m_projectionMatrix;
    float m_yaw;
    float m_pitch;
};
}//Cameras
}//McEngine

