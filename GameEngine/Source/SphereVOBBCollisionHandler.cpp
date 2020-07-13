#include "SphereVOBBCollisionHandler.h"
#include <algorithm>
#include <iostream>

namespace McEngine
{
namespace Physics
{

SphereVOBBCollisionHandler::SphereVOBBCollisionHandler(const glm::vec3 & p_objectCenterA, 
                                                       const glm::vec3 & p_objectCenterB,
                                                       glm::vec3& p_collisionDirection)
    :m_objectCenterA(p_objectCenterA), 
     m_objectCenterB(p_objectCenterB), 
     m_collsionDirection(p_collisionDirection)
{
}

bool SphereVOBBCollisionHandler::checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB)
{
    glm::vec3 l_closestPointOnOBBCube;
    float l_distance = 0.0f;
    float l_radius = 0.0f;
    if (p_coliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        l_distance = calculateDistanceBetweenClosestPointAndSphereCeneter(p_coliderA, p_coliderB, m_objectCenterB);
        l_radius = p_coliderB.m_radius;
    }
    else
    {
        l_distance = calculateDistanceBetweenClosestPointAndSphereCeneter(p_coliderB, p_coliderA, m_objectCenterA);
        l_radius = p_coliderA.m_radius;
    }

    return l_distance < l_radius;
}

float SphereVOBBCollisionHandler::calculateDistanceBetweenClosestPointAndSphereCeneter(const Meshes::Collider & p_cubeCollider, 
                                                                                       const Meshes::Collider & p_sphereCoolider,
                                                                                       const glm::vec3& p_sphereCenter)
{
    float l_distance = 0.0f;
    glm::vec3 l_closestPointOnOBBCube;
    glm::mat4 l_inverseTransformPoint = glm::inverse(p_cubeCollider.m_modelMatrix);
    glm::vec4 l_localSphereCenter = l_inverseTransformPoint * glm::vec4((p_sphereCenter + p_sphereCoolider.m_transform.m_position), 1.0f);

    glm::vec4 l_localXSectionMin = l_inverseTransformPoint * p_cubeCollider.m_xSection.min;
    glm::vec4 l_localXSectionMax = l_inverseTransformPoint * p_cubeCollider.m_xSection.max;

    glm::vec4 l_localYSectionMin = l_inverseTransformPoint * p_cubeCollider.m_ySection.min;
    glm::vec4 l_localYSectionMax = l_inverseTransformPoint * p_cubeCollider.m_ySection.max;

    glm::vec4 l_localZSectionMin = l_inverseTransformPoint * p_cubeCollider.m_zSection.min;
    glm::vec4 l_localZSectionMax = l_inverseTransformPoint * p_cubeCollider.m_zSection.max;
    l_closestPointOnOBBCube =
        glm::vec3(
            std::max(l_localXSectionMin.x, std::min(l_localSphereCenter.x, l_localXSectionMax.x)),
            std::max(l_localYSectionMin.y, std::min(l_localSphereCenter.y, l_localYSectionMax.y)),
            std::min(l_localZSectionMin.z, std::max(l_localSphereCenter.z, l_localZSectionMax.z)));
    l_distance = glm::distance(l_closestPointOnOBBCube, glm::vec3(l_localSphereCenter));
    m_collsionDirection = glm::normalize(l_closestPointOnOBBCube - p_sphereCoolider.m_transform.m_position);

    return l_distance;
}

}//Physics
}//McEngine