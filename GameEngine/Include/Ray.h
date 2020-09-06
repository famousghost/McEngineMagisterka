#pragma once
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
class Ray
{
public:
    Ray();
    Ray(const glm::vec3& p_origin, const glm::vec3& p_direction);

    void normalizeDirection();

    glm::vec3 m_origin;
    glm::vec3 m_direction;
};
}//Meshes
}//McEngine

