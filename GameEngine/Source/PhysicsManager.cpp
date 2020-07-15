#include "PhysicsManager.h"
#include <algorithm>
#include <math.h>

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

bool PhysicsManager::getShouldCheckCollsion() const
{
    return m_collsionHandler.getShouldCheckCollsion();
}

void PhysicsManager::setShouldCheckCollision(bool p_shouldCheckCollision)
{
    m_collsionHandler.setShouldCheckCollision(p_shouldCheckCollision);
}

void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    if(getShouldCheckCollsion() or p_object.m_isRigidBody)
    {
        m_collsionHandler.collisionChecker(p_object, p_objects);
    }
}

}//Physics
}//McEngine
