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

    void update(Shaders::Shader & p_shaderProgram, 
                std::string p_cameraPostionUniform, 
                std::string p_viewMatrixUniform);

    glm::vec3 getCameraPosition() const;

    glm::mat4 getViewMatrix() const;
private:
    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::mat4 m_view;
    float m_yaw;
    float m_pitch;
};
}//Cameras
}//McEngine

