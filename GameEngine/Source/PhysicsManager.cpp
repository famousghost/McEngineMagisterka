#include "PhysicsManager.h"
#include "TimeManager.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{

namespace
{
    constexpr int STATE_SIZE = 18;
}

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

void PhysicsManager::updateBodyState(Meshes::Rigidbody& p_rigidBody)
{
    auto l_deltaTime = static_cast<float>(Time::TimeManager::getInstance().getDeltaTime());
    p_rigidBody.m_x = p_rigidBody.m_dx;
    p_rigidBody.m_R = p_rigidBody.m_dR;
    p_rigidBody.m_P = p_rigidBody.m_dP;
    p_rigidBody.m_L = p_rigidBody.m_dL;

    p_rigidBody.m_velocity = glm::vec3(p_rigidBody.m_P) * p_rigidBody.m_massProperties.m_inverseMass * l_deltaTime;

    p_rigidBody.m_iInv = p_rigidBody.m_R * p_rigidBody.m_inverseIbody * glm::transpose(p_rigidBody.m_R);

    p_rigidBody.m_angularVelocity = p_rigidBody.m_iInv * p_rigidBody.m_L * l_deltaTime;
}

void PhysicsManager::dbgVector(const glm::vec3& p_vec, const std::string& p_msg)
{
    std::cout << p_msg << "(" << p_vec.x << ", " << p_vec.y << ", " << p_vec.z << ")" << std::endl;
}

void PhysicsManager::ode(Meshes::Object& p_object)
{
    auto& l_rigidBody = p_object.m_rigidBody;
    updateBodyState(l_rigidBody);
    computeForceAndTorque(p_object);
    calculateDerivates(l_rigidBody);
}

void PhysicsManager::computeForceAndTorque(Meshes::Object& p_object)
{
    if (p_object.m_isRigidBody and p_object.m_gravityForce)
    {
        auto l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();
        auto& l_rigidBody = p_object.m_rigidBody;
        l_rigidBody.m_force.y += l_rigidBody.m_gravity * l_rigidBody.m_massProperties.m_mass * l_deltaTime;
        auto& l_collider = p_object.m_colider.at(0);
        glm::vec3 l_centerOfMass = p_object.m_transform.m_position + l_collider.m_transform.m_position;
        auto r = l_rigidBody.m_x + (l_rigidBody.m_R * l_centerOfMass);
        l_rigidBody.m_torque = glm::cross(r - l_rigidBody.m_x, l_rigidBody.m_force);
    }
}

glm::mat3 PhysicsManager::starOperatorMatrix(const glm::vec3& p_vec)
{
    glm::mat3 l_result;
    
    l_result[0][1] = -p_vec.z;
    l_result[0][2] = p_vec.y;
    l_result[1][0] = p_vec.z;
    l_result[1][2] = -p_vec.x;
    l_result[2][0] = -p_vec.y;
    l_result[2][1] = p_vec.x;

    return l_result;
}

void PhysicsManager::debugMatrix(const glm::dmat3& p_mat)
{
    for (auto i = 0; i < 3; ++i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            std::cout <<"R["<< i << "]" << "[" << j << "] = " << p_mat[i][j];
        }
        std::cout << std::endl;
    }
}

void PhysicsManager::calculateDerivates(Meshes::Rigidbody& p_rigidBody)
{
    p_rigidBody.m_dx = p_rigidBody.m_velocity;
    p_rigidBody.m_dP = p_rigidBody.m_force;
    p_rigidBody.m_dL = p_rigidBody.m_torque;

    glm::mat3 l_rDot = starOperatorMatrix(p_rigidBody.m_angularVelocity) * p_rigidBody.m_R;
    debugMatrix(l_rDot);
    p_rigidBody.m_dR = l_rDot;
}

void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    m_collsionHandler.collisionChecker(p_object, p_objects);
}

}//Physics
}//McEngine
