#include "SphereCollsionHandler.h"

namespace McEngine
{
namespace Physics
{

SphereCollsionHandler::SphereCollsionHandler(const glm::vec3 & p_objectCenterA, 
                                             const glm::vec3 & p_objectCenterB)
    :m_objectCenterA(p_objectCenterA), m_objectCenterB(p_objectCenterB)
{
}

bool SphereCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA,
                                           const Meshes::Collider & p_coliderB)
{
    auto l_distance = glm::distance(m_objectCenterA, m_objectCenterB);
    auto l_distanceVec = m_objectCenterA - m_objectCenterB;
    auto l_radiusCombination = p_coliderA.m_radius + p_coliderB.m_radius;
    auto l_dir = glm::normalize(l_distanceVec);
    m_colMainfold.m_isColliding = true;
    m_colMainfold.m_normal = l_dir;
    m_colMainfold.m_depth = fabsf(l_distance - l_radiusCombination) * 0.5f;
    float l_distanceToPoint = p_coliderA.m_radius - m_colMainfold.m_depth;
    glm::vec3 l_contactPoint = m_objectCenterA + l_dir * l_distanceToPoint;
    m_colMainfold.m_contacts.push_back(l_contactPoint);
    return l_radiusCombination > l_distance;
}

Meshes::ColMainfold SphereCollsionHandler::getColMainfold() const
{
    return m_colMainfold;
}

}//Physics
}//McEngine