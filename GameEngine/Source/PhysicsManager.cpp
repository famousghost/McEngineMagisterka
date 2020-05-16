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
    auto l_coliderAMaxValueX = p_coliderA.m_firstVertex.x + p_coliderA.m_width;
    auto l_coliderAMaxValueY = p_coliderA.m_firstVertex.y + p_coliderA.m_heigth;
    auto l_coliderAMaxValueZ = p_coliderA.m_firstVertex.z + p_coliderA.m_length;

    auto l_coliderBMaxValueX = p_coliderB.m_firstVertex.x + p_coliderB.m_width;
    auto l_coliderBMaxValueY = p_coliderB.m_firstVertex.y + p_coliderB.m_heigth;
    auto l_coliderBMaxValueZ = p_coliderB.m_firstVertex.z + p_coliderB.m_length;

    return (p_coliderA.m_firstVertex.x <= l_coliderBMaxValueX
           and l_coliderAMaxValueX >= p_coliderB.m_firstVertex.x)
           and (p_coliderA.m_firstVertex.y <= l_coliderBMaxValueY
           and l_coliderAMaxValueY >= p_coliderB.m_firstVertex.y)
           and (p_coliderA.m_firstVertex.z <= l_coliderBMaxValueZ
           and l_coliderAMaxValueZ >= p_coliderB.m_firstVertex.z);
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
