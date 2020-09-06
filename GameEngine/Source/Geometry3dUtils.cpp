#include "Geometry3dUtils.h"
#include <math.h>

namespace McEngine
{
namespace Utils
{ 

Ray Geometry3dUtils::fromPoints(const glm::vec3 & p_from, const glm::vec3 & p_to)
{
    return Ray(p_from, glm::normalize(p_to - p_from));
}

bool Geometry3dUtils::pointOnRay(const glm::vec3& p_point, const Ray& p_ray)
{
    if (p_point == p_ray.m_origin)
    {
        return true;
    }
    auto l_normalFromPointToOrigin = glm::normalize(p_point - p_ray.m_origin);

    return glm::dot(l_normalFromPointToOrigin, p_ray.m_direction) == 1.0f;

}

glm::vec3 Geometry3dUtils::closestPoint(const Ray& p_ray, const glm::vec3& p_point)
{
    auto l_t = glm::dot(p_point - p_ray.m_origin, p_ray.m_direction);

    l_t /= (glm::length(p_ray.m_direction) * glm::length(p_ray.m_direction));

    l_t = std::fmax(l_t, 0.0f);

    return p_ray.m_origin + p_ray.m_direction * l_t;
}

float Geometry3dUtils::raycast(const Meshes::Object & p_object, const Ray & p_ray)
{
    auto& l_collider = p_object.m_colider.at(0);
    if (l_collider.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        auto l_vecFromOriginToSphereCenter = p_object.m_transform.m_position - p_ray.m_origin;

        float l_magnitudeOfVecSquare = glm::length(l_vecFromOriginToSphereCenter) * glm::length(l_vecFromOriginToSphereCenter);
        float l_radiusSquare = l_collider.m_radius * l_collider.m_radius;

        float l_castVecToRay = glm::dot(l_vecFromOriginToSphereCenter, p_ray.m_direction);

        auto l_bSq = l_magnitudeOfVecSquare - l_castVecToRay * l_castVecToRay;
        auto l_f = std::sqrt(l_radiusSquare - l_bSq);
        if (l_radiusSquare - l_bSq < 0.0f)
        {
            return -1.0f;
        }
        if (l_magnitudeOfVecSquare < l_radiusSquare)
        {
            return l_castVecToRay + l_f;
        }
        return l_castVecToRay - l_f;
    }
    if(l_collider.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {

    }
    return 0.0f;
}

}//Utils
}//McEngine
