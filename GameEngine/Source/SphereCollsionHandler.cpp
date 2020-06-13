#include "SphereCollsionHandler.h"

namespace McEngine
{
namespace Physics
{
bool SphereCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA, 
                                           const Meshes::Collider & p_coliderB)
{
    return (p_coliderA.m_radius + p_coliderB.m_radius)
            > glm::distance(p_coliderA.m_transform.m_position, p_coliderB.m_transform.m_position);
}
}//Physics
}//McEngine