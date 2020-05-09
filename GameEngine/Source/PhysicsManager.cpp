#include "PhysicsManager.h"

namespace McEngine
{
namespace Physics
{

void PhysicsManager::start()
{
}

void PhysicsManager::shutdown()
{
}

PhysicsManager & PhysicsManager::getInstance()
{
    static PhysicsManager l_physicsManager;
    return l_physicsManager;
}

bool PhysicsManager::checkCollision(const Meshes::Colider& p_coliderA, 
                                    const Meshes::Colider& p_coliderB)
{
    return true;
}

}//Physics
}//McEngine
