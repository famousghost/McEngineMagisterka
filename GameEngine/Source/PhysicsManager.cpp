#include "PhysicsManager.h"
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

}//Physics
}//McEngine
