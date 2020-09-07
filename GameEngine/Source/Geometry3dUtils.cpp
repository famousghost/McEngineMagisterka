#include "Geometry3dUtils.h"
#include <array>
#include <math.h>

namespace McEngine
{
namespace Utils
{ 

namespace
{
    constexpr std::size_t MAX_MIN_VALUES_SIZE = 6;
}

float Geometry3dUtils::cmp(float x, float y)
{
    return std::fabsf(x - y) 
           <= FLT_EPSILON * std::fmaxf(1.0f, std::fmaxf(std::fabsf(x), std::fabsf(y)));
}

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

float Geometry3dUtils::raycastSphere(const Meshes::Object& p_object, 
                                     const Meshes::Collider& p_collider, 
                                     const Ray& p_ray)
{
    auto l_vecFromOriginToSphereCenter = p_object.m_transform.m_position - p_ray.m_origin;

    float l_magnitudeOfVecSquare = glm::length(l_vecFromOriginToSphereCenter) * glm::length(l_vecFromOriginToSphereCenter);
    float l_radiusSquare = p_collider.m_radius * p_collider.m_radius;

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

float Geometry3dUtils::findClosestPointOnCube(glm::vec3 p_max, glm::vec3 p_min)
{
    float l_maxMin = std::fmax(std::fmax(std::fmin(p_min.x, p_max.x), std::fmin(p_min.y, p_max.y)), std::fmin(p_min.z, p_max.z));
    float l_minMax = std::fmin(std::fmin(std::fmax(p_min.x, p_max.x), std::fmax(p_min.y, p_max.y)), std::fmax(p_min.z, p_max.z));

    if (l_minMax < 0.0f)
    {
        return -1.0f;
    }
    if (l_maxMin > l_minMax)
    {
        return -1.0f;
    }
    if (l_maxMin < 0.0f)
    {
        return l_minMax;
    }
    return l_maxMin;
}

float Geometry3dUtils::raycastAABB(const Meshes::Object & p_object, 
                                   const Meshes::Collider& p_collider, 
                                   const Ray & p_ray)
{
    auto l_min = p_collider.m_minVertex;
    auto l_max = p_collider.m_maxVertex;
    float txmin = 0.0f;
    float txmax = 0.0f;
    float tymin = 0.0f;
    float tymax = 0.0f;
    float tzmin = 0.0f;
    float tzmax = 0.0f;
    if (p_ray.m_direction.x != 0.0f)
    {
        txmin = (l_min.x - p_ray.m_origin.x) / p_ray.m_direction.x;
        txmax = (l_max.x - p_ray.m_origin.x) / p_ray.m_direction.x;
    }
    if (p_ray.m_direction.y != 0.0f)
    {
        tymin = (l_min.y - p_ray.m_origin.y) / p_ray.m_direction.y;
        tymax = (l_max.y - p_ray.m_origin.y) / p_ray.m_direction.y;
    }
    if (p_ray.m_direction.z != 0.0f)
    {
        tzmin = (l_min.z - p_ray.m_origin.z) / p_ray.m_direction.z;
        tzmax = (l_max.z - p_ray.m_origin.z) / p_ray.m_direction.z;
    }

    return findClosestPointOnCube(glm::vec3(txmax, tymax, tzmax), glm::vec3(txmin, tymin, tzmin));
}

float Geometry3dUtils::raycastOBB(const Meshes::Object & p_object,
                                  const Meshes::Collider& p_collider,
                                  const Ray & p_ray)
{
    glm::vec3 l_size = glm::vec3(p_object.m_rigidBody.m_width, 
                                 p_object.m_rigidBody.m_height, 
                                 p_object.m_rigidBody.m_length);

    auto& l_orientation = p_object.m_transform.m_orientation;
    glm::vec3 l_x = glm::vec3(l_orientation[0][0], l_orientation[0][1], l_orientation[0][2]);
    glm::vec3 l_y = glm::vec3(l_orientation[1][0], l_orientation[1][1], l_orientation[1][2]);
    glm::vec3 l_z = glm::vec3(l_orientation[2][0], l_orientation[2][1], l_orientation[2][2]);
    
    glm::vec3 l_p = p_object.m_transform.m_position - p_ray.m_origin;

    glm::vec3 l_projectionOfDirectionToEachAxis = glm::vec3(glm::dot(l_x, p_ray.m_direction),
                                                            glm::dot(l_y, p_ray.m_direction),
                                                            glm::dot(l_z, p_ray.m_direction));

    glm::vec3 l_projectionOfPToEachAxis = glm::vec3(glm::dot(l_x, l_p),
                                                    glm::dot(l_y, l_p),
                                                    glm::dot(l_z, l_p));


    std::array<float, MAX_MIN_VALUES_SIZE> l_t {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 3; ++i)
    {
        if (cmp(l_projectionOfDirectionToEachAxis[i], 0))
        {
            if (-l_projectionOfPToEachAxis[i] - l_size[i] > 0 
                || -l_projectionOfPToEachAxis[i] + l_size[i] < 0) 
            {
                return -1;
            }
            l_projectionOfDirectionToEachAxis[i] = 0.00001f;
        }
        l_t[i * 2 + 0] = 
            (l_projectionOfPToEachAxis[i] + l_size[i]) / l_projectionOfDirectionToEachAxis[i]; // min
        l_t[i * 2 + 1] = 
            (l_projectionOfPToEachAxis[i] - l_size[i]) / l_projectionOfDirectionToEachAxis[i]; // max
    }

    return findClosestPointOnCube(glm::vec3(l_t[0], l_t[2], l_t[4]), glm::vec3(l_t[1], l_t[3], l_t[5]));
}

float Geometry3dUtils::raycast(const Meshes::Object & p_object, const Ray & p_ray)
{
    auto& l_collider = p_object.m_colider.at(0);
    if (l_collider.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        return raycastSphere(p_object, l_collider, p_ray);
    }
    if(l_collider.m_colliderType == Meshes::ColliderType::CUBE_AABB)
    {
        return raycastAABB(p_object, l_collider, p_ray);
    }
    if (l_collider.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        return raycastOBB(p_object, l_collider, p_ray);
    }
    return -1.0f;
}

}//Utils
}//McEngine
