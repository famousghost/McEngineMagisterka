#pragma once
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
struct Plane
{
    glm::vec3 m_normal;
    float m_distance;

    Plane() = default;

    Plane(const glm::vec3& p_normal, float p_distance)
        :m_normal(p_normal), m_distance(p_distance)
    {}
};

struct PlaneHandler
{
    static float planeEquation(const glm::vec3& p_point, const Plane& p_plane)
    {
        return glm::dot(p_point, p_plane.m_normal) - p_plane.m_distance;
    }
};

}//Meshes
}//McEngine

