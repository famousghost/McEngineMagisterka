#pragma once
#include "Utilities.h"

namespace McEngine
{
namespace Meshes
{
struct Light
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    glm::vec3 m_position;

    Light()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};
}//Meshes
}//McEngine
