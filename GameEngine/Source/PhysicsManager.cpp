#include "PhysicsManager.h"
#include <math.h>

namespace McEngine
{
namespace Physics
{

void PhysicsManager::start()
{
    m_shouldCheckCollsion = false;
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
    return (p_coliderA.m_xSection.min.x <= p_coliderB.m_xSection.max.x
           and p_coliderA.m_xSection.max.x >= p_coliderB.m_xSection.min.x)
           and (p_coliderA.m_ySection.min.y <= p_coliderB.m_ySection.max.y
           and p_coliderA.m_ySection.max.y >= p_coliderB.m_ySection.min.y)
           and (p_coliderA.m_zSection.min.z >= p_coliderB.m_zSection.max.z
           and p_coliderA.m_zSection.max.z <= p_coliderB.m_zSection.min.z);
}

bool PhysicsManager::getShouldCheckCollsion() const
{
    return m_shouldCheckCollsion;
}

void PhysicsManager::setShouldCheckCollision(bool p_shouldCheckCollision)
{
    m_shouldCheckCollsion = p_shouldCheckCollision;
}

void PhysicsManager::checkCollisions(Meshes::Object& p_object, 
                                     std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    if(m_shouldCheckCollsion)
    {
        std::cout << p_object.m_objectName << "Check collision" << std::endl;
        collisionChecker(p_object, p_objects);
    }
}

void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool isColliding = false;
    for (std::size_t j = 0; j < p_objects.size(); ++j)
    {
        if (p_object.m_objectName == p_objects[j].first.m_objectName)
        {
            continue;
        }

        if (checkCollision(p_object.m_colider, p_objects[j].first.m_colider))
        {
            p_object.m_colider.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
            isColliding = true;
        }
        
    }
    if (not isColliding)
    {
        p_object.m_colider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

}//Physics
}//McEngine
