#include "SphereVOBBCollisionHandler.h"
namespace McEngine
{
namespace Physics
{
    SphereVOBBCollisionHandler::SphereVOBBCollisionHandler(float p_distance)
        :m_distance(p_distance)
    {
    }

    bool SphereVOBBCollisionHandler::checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB)
    {
        return false;
    }
}//Physics
}//McEngine