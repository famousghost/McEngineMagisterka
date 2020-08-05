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

void PhysicsManager::stateToArray(Meshes::Rigidbody& p_rigidBody, std::vector<double>& p_y)
{
    auto l_it = p_y.begin();
    *l_it++ = p_rigidBody.m_x.x;
    *l_it++ = p_rigidBody.m_x.y;
    *l_it++ = p_rigidBody.m_x.z;

    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            *l_it++ = p_rigidBody.m_R[i][j];
        }
    }

    *l_it++ = p_rigidBody.m_P.x;
    *l_it++ = p_rigidBody.m_P.y;
    *l_it++ = p_rigidBody.m_P.z;

    *l_it++ = p_rigidBody.m_L.x;
    *l_it++ = p_rigidBody.m_L.y;
    *l_it++ = p_rigidBody.m_L.z;
}

void PhysicsManager::arrayToState(Meshes::Rigidbody& p_rigidBody, std::vector<double>& p_y)
{
    auto l_it = p_y.begin();
    auto l_deltaTime = static_cast<float>(Time::TimeManager::getInstance().getDeltaTime());
    p_rigidBody.m_x.x = *l_it++;
    p_rigidBody.m_x.y = *l_it++;
    p_rigidBody.m_x.z = *l_it++;

    for (auto i = 0; i < 3; i++)
    {
        for (auto j = 0; j < 3; j++)
        {
            p_rigidBody.m_R[i][j] = *l_it++;
        }
    }

    p_rigidBody.m_P.x = *l_it++;
    p_rigidBody.m_P.y = *l_it++;
    p_rigidBody.m_P.z = *l_it++;

    p_rigidBody.m_L.x = *l_it++;
    p_rigidBody.m_L.y = *l_it++;
    p_rigidBody.m_L.z = *l_it++;

    p_rigidBody.m_velocity = glm::vec3(p_rigidBody.m_P) * p_rigidBody.m_massProperties.m_inverseMass * l_deltaTime;

    p_rigidBody.m_iInv = p_rigidBody.m_R * p_rigidBody.m_inverseIbody * glm::transpose(p_rigidBody.m_R);

    p_rigidBody.m_angularVelocity = p_rigidBody.m_iInv * p_rigidBody.m_L * static_cast<double>(l_deltaTime);
}

void PhysicsManager::dbgVector(const glm::vec3& p_vec, const std::string& p_msg)
{
    std::cout << p_msg << "(" << p_vec.x << ", " << p_vec.y << ", " << p_vec.z << ")" << std::endl;
}

void PhysicsManager::arrayToBody(Meshes::Rigidbody& p_rigidBody, std::vector<double>& p_y)
{
    arrayToState(p_rigidBody, p_y);
}

void PhysicsManager::bodyToArray(Meshes::Rigidbody& p_rigidBody, std::vector<double>& p_y)
{
    stateToArray(p_rigidBody, p_y);
}

void PhysicsManager::dydt(Meshes::Object& p_rigidBody, double p_t)
{
    auto& l_rigidBody = p_rigidBody.m_rigidBody;
    arrayToBody(l_rigidBody, l_rigidBody.m_y);
    computeForceAndTorque(p_t, p_rigidBody);
    calculateDerivates(l_rigidBody, l_rigidBody.m_yFinal);
}

void PhysicsManager::ode(Meshes::Object& p_object)
{
    auto l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();
    dydt(p_object, l_deltaTime);
}

void PhysicsManager::computeForceAndTorque(double p_t, Meshes::Object& p_object)
{
    if (p_object.m_isRigidBody and p_object.m_gravityForce)
    {
        auto& l_rigidBody = p_object.m_rigidBody;
        l_rigidBody.m_force.y += l_rigidBody.m_gravity * l_rigidBody.m_massProperties.m_mass;
        auto& l_collider = p_object.m_colider.at(0);
        glm::vec3 l_centerOfMass = p_object.m_transform.m_position + l_collider.m_transform.m_position;
        auto r = l_rigidBody.m_x + (l_rigidBody.m_R * l_centerOfMass);
        l_rigidBody.m_torque = glm::cross(r - l_rigidBody.m_x, l_rigidBody.m_force);
    }
}

glm::dmat3 PhysicsManager::starOperatorMatrix(const glm::vec3& p_vec)
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

void PhysicsManager::calculateDerivates(Meshes::Rigidbody& p_rigidBody, std::vector<double>& p_y)
{
    auto l_it = p_y.begin();
    *l_it++ = p_rigidBody.m_velocity.x;
    *l_it++ = p_rigidBody.m_velocity.y;
    *l_it++ = p_rigidBody.m_velocity.z;

    glm::mat3 l_rDot = starOperatorMatrix(p_rigidBody.m_angularVelocity) * p_rigidBody.m_R;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            *l_it++ = l_rDot[i][j];

    debugMatrix(l_rDot);


    *l_it++ = p_rigidBody.m_force.x;
    *l_it++ = p_rigidBody.m_force.y;
    *l_it++ = p_rigidBody.m_force.z;

    *l_it++ = p_rigidBody.m_torque.x;
    *l_it++ = p_rigidBody.m_torque.y;
    *l_it++ = p_rigidBody.m_torque.z;
}

void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    m_collsionHandler.collisionChecker(p_object, p_objects);
}

}//Physics
}//McEngine
