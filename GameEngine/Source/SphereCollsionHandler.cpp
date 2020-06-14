#include "SphereCollsionHandler.h"

namespace McEngine
{
namespace Physics
{

SphereCollsionHandler::SphereCollsionHandler(float p_distance)
    :m_distance(p_distance)
{
}

bool SphereCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA,
                                           const Meshes::Collider & p_coliderB)
{
    return (p_coliderA.m_radius + p_coliderB.m_radius) > m_distance;
}

}//Physics
}//McEngine