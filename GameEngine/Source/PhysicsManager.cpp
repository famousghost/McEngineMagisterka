#include "PhysicsManager.h"
#include "TimeManager.h"
#include "RigidbodyHandlerOBB.h"
#include "Geometry3dUtils.h"
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

void PhysicsManager::calculateObjectMass(Meshes::Object& p_object)
{
    p_object.m_rigidBody.m_massProperties.m_mass = 
        p_object.m_rigidBody.m_materialProperties.m_density * calculateObjectVolume(p_object);
    p_object.m_rigidBody.m_massProperties.m_inverseMass = 1.0f / p_object.m_rigidBody.m_massProperties.m_mass;
    LOG(std::to_string(p_object.m_rigidBody.m_massProperties.m_mass), LogType::DBG);
}

float PhysicsManager::calculateObjectVolume(Meshes::Object& p_object)
{
    return fabs(p_object.m_rigidBody.m_width) * fabs(p_object.m_rigidBody.m_height) * fabs(p_object.m_rigidBody.m_length);
}

bool PhysicsManager::getShouldCheckCollsion() const
{
    return m_collsionHandler.getShouldCheckCollsion();
}

void PhysicsManager::updatePhysics(Meshes::Object& p_object,
                                   std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    collisionChecker(p_object, p_objects);
}

int PhysicsManager::getStateSize() const
{
    return m_stateSize;
}

void PhysicsManager::setShouldCheckCollision(bool p_shouldCheckCollision)
{
    m_collsionHandler.setShouldCheckCollision(p_shouldCheckCollision);
}

void PhysicsManager::ode(Meshes::Object& p_object)
{
    computeForceAndTorque(p_object);
}

void PhysicsManager::computeForceAndTorque(Meshes::Object& p_object)
{
    std::shared_ptr<IRigidbodyHandler> m_rigidbodyHandler;

    if (p_object.m_colider.at(0).m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        m_rigidbodyHandler = std::make_shared<RigidbodyHandlerOBB>(&p_object.m_rigidBody);
    }
    else if (p_object.m_colider.at(0).m_colliderType == Meshes::ColliderType::SPHERE)
    {
        m_rigidbodyHandler = std::make_shared<RigidbodyHandlerOBB>(&p_object.m_rigidBody);
    }

    m_rigidbodyHandler->update();
}


void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    m_collsionHandler.collisionChecker(p_object, p_objects);
}

}//Physics
}//McEngine
