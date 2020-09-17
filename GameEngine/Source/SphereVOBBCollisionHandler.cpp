#include "SphereVOBBCollisionHandler.h"
#include "Geometry3dUtils.h"
#include <algorithm>
#include <iostream>

namespace McEngine
{
namespace Physics
{

SphereVOBBCollisionHandler::SphereVOBBCollisionHandler(const glm::vec3 & p_objectCenterA, 
                                                       const glm::vec3 & p_objectCenterB,
                                                       glm::vec3& p_collisionDirection,
                                                       const Meshes::Object* p_cube)
    :m_cubeCenter(p_objectCenterA), 
     m_sphereCenter(p_objectCenterB), 
     m_collsionDirection(p_collisionDirection),
     m_cube(p_cube)
{
}

bool SphereVOBBCollisionHandler::checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB)
{
    glm::vec3 l_closestPointOnOBBCube;
    float l_distance = 0.0f;
    float l_radius = 0.0f;
    l_distance = calculateDistanceBetweenClosestPointAndSphereCeneter(p_coliderA, p_coliderB, m_sphereCenter, m_cubeCenter);
    l_radius = p_coliderB.m_radius;

    return l_distance < l_radius;
}

Meshes::ColMainfold SphereVOBBCollisionHandler::getColMainfold() const
{
    return m_colMainfold;
}

float SphereVOBBCollisionHandler::calculateDistanceBetweenClosestPointAndSphereCeneter(const Meshes::Collider & p_cubeCollider, 
                                                                                       const Meshes::Collider & p_sphereCoolider,
                                                                                       const glm::vec3& p_sphereCenter,
                                                                                       const glm::vec3& p_cubePosition)
{
    glm::vec3 l_closestPointOnOBBCube =  Utils::Geometry3dUtils::findClosestPointOnCubeOBB(*m_cube, p_sphereCenter);

    float l_distanceSq = 
        glm::length(l_closestPointOnOBBCube - p_sphereCenter) * 
        glm::length(l_closestPointOnOBBCube - p_sphereCenter);

    if (l_distanceSq > (p_sphereCoolider.m_radius * p_sphereCoolider.m_radius)) 
    {
        return -1.0f;
    }

    glm::vec3 l_normal;
    if(Utils::Geometry3dUtils::cmp(l_distanceSq, 0.0f))
    {
        float l_mSq = glm::length(l_closestPointOnOBBCube - p_cubePosition) *
                      glm::length(l_closestPointOnOBBCube - p_cubePosition);
        if (Utils::Geometry3dUtils::cmp(l_mSq, 0.0f))
        {
            return -1.0f;
        }
        l_normal = glm::normalize(l_closestPointOnOBBCube - p_cubePosition);
    }
    else
    {
        l_normal = glm::normalize(p_sphereCenter - l_closestPointOnOBBCube);
    }

    glm::vec3 l_outsidePoint = p_sphereCenter - l_normal * p_sphereCoolider.m_radius;
    float l_distanceBetweenOutsidePointAndClosestPoint = glm::length(l_closestPointOnOBBCube - l_outsidePoint);
    m_colMainfold.m_isColliding = true;
    m_colMainfold.m_contacts.push_back(l_closestPointOnOBBCube + (l_outsidePoint - l_closestPointOnOBBCube) * 0.5f);
    m_colMainfold.m_normal = l_normal;
    m_colMainfold.m_depth = l_distanceBetweenOutsidePointAndClosestPoint * 0.5f;

    return l_distanceBetweenOutsidePointAndClosestPoint;
}

}//Physics
}//McEngine