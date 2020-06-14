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
    return (p_coliderA.m_radius + p_coliderB.m_radius) > l_distance;
}

}//Physics
}//McEngine