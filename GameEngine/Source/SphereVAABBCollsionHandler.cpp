#include "SphereVAABBCollsionHandler.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{

SphereVAABBCollsionHandler::SphereVAABBCollsionHandler(const glm::vec3 & p_objectCenterA, const glm::vec3 & p_objectCenterB)
    :m_objectCenterA(p_objectCenterA), m_objectCenterB(p_objectCenterB)
{
}

bool SphereVAABBCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB)
{
    glm::vec3 l_closestPointOnAABBCube;
    float l_distance;
    float l_radius;
    if (p_coliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB)
    {
        glm::vec3 l_sphereCenter = m_objectCenterB + p_coliderB.m_transform.m_position;
        l_closestPointOnAABBCube = 
            glm::vec3(
                std::max(p_coliderA.m_xSection.min.x, std::min(l_sphereCenter.x, p_coliderA.m_xSection.max.x)),
                std::max(p_coliderA.m_ySection.min.y, std::min(l_sphereCenter.y, p_coliderA.m_ySection.max.y)),
                std::min(p_coliderA.m_zSection.min.z, std::max(l_sphereCenter.z, p_coliderA.m_zSection.max.z)));
        l_distance = glm::distance(l_closestPointOnAABBCube, l_sphereCenter);
        l_radius = p_coliderB.m_radius;
    }
    else
    {
        glm::vec3 l_sphereCenter = m_objectCenterA + p_coliderA.m_transform.m_position;
        l_closestPointOnAABBCube =
            glm::vec3(
                std::max(p_coliderB.m_xSection.min.x, std::min(l_sphereCenter.x, p_coliderB.m_xSection.max.x)),
                std::max(p_coliderB.m_ySection.min.y, std::min(l_sphereCenter.y, p_coliderB.m_ySection.max.y)),
                std::min(p_coliderB.m_zSection.min.z, std::max(l_sphereCenter.z, p_coliderB.m_zSection.max.z)));
        l_distance = glm::distance(l_closestPointOnAABBCube, l_sphereCenter);
        l_radius = p_coliderA.m_radius;
    }

    return l_distance < l_radius;
}

}//Physics
}//McEngine