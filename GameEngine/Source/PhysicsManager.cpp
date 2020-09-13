#include "PhysicsManager.h"
#include "TimeManager.h"
#include "RigidbodyHandlerOBB.h"
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

void PhysicsManager::updateBodyState(Meshes::Rigidbody& p_rigidBody)
{
    auto l_deltaTime = static_cast<float>(Time::TimeManager::getInstance().getDeltaTime());
    p_rigidBody.m_prevPosition = *p_rigidBody.m_position;
    *p_rigidBody.m_position += p_rigidBody.m_dx * l_deltaTime;
    dbgVector(*p_rigidBody.m_position, "RIGIDBODY POS = ");
    p_rigidBody.m_P += p_rigidBody.m_dP * l_deltaTime;
    p_rigidBody.m_L += p_rigidBody.m_dL * l_deltaTime;
    p_rigidBody.m_quat += glm::normalize(p_rigidBody.m_quatDt) * l_deltaTime;

    p_rigidBody.m_oldVelocity = p_rigidBody.m_velocity;
    p_rigidBody.m_velocity = glm::vec3(p_rigidBody.m_P) * p_rigidBody.m_massProperties.m_inverseMass;

    p_rigidBody.m_iWordlInv = glm::toMat3(p_rigidBody.m_quat) * p_rigidBody.m_inverseIbody * glm::transpose(glm::toMat3(p_rigidBody.m_quat));

    p_rigidBody.m_angularVelocity = p_rigidBody.m_iWordlInv * p_rigidBody.m_L;
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
    computeTorque(p_object);
}

void PhysicsManager::computeTorque(Meshes::Object& p_object)
{
    auto l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();
    auto& l_rigidBody = p_object.m_rigidBody;
    glm::vec3 l_centerOfMass = glm::vec3(0.0f, 0.5f*l_rigidBody.m_height, 0.0f);
    auto r = *l_rigidBody.m_position + (glm::toMat3(l_rigidBody.m_quat) * l_centerOfMass);
    l_rigidBody.m_torque = 
    glm::cross(r - *l_rigidBody.m_position, l_rigidBody.m_force);
}

glm::mat4 PhysicsManager::starOperatorMatrix(const glm::vec3& p_vec)
{
    glm::mat4 l_result = glm::mat4();
    
    l_result[0][0] = 0.0f;
    l_result[0][1] = -p_vec.z;
    l_result[0][2] = p_vec.y;
    l_result[1][0] = p_vec.z;
    l_result[1][1] = 0.0f;
    l_result[1][2] = -p_vec.x;
    l_result[2][0] = -p_vec.y;
    l_result[2][1] = p_vec.x;
    l_result[2][2] = 0.0f;

    return l_result;
}

void PhysicsManager::debugMatrix(const glm::mat4& p_mat)
{
    std::cout << "-----------------------------------" << std::endl;
    for (auto i = 0; i < 3; ++i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            std::cout <<"R["<< i << "]" << "[" << j << "] = " << p_mat[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
}

void PhysicsManager::debugQuat(const glm::quat& p_quat)
{
    std::cout << "QUAT: " << "(" << p_quat.x << ", " << p_quat.y << ", " << p_quat.z << p_quat.w << ")" << std::endl;
}

void PhysicsManager::calculateDerivates(Meshes::Rigidbody& p_rigidBody)
{
    p_rigidBody.m_dx = (p_rigidBody.m_velocity + p_rigidBody.m_oldVelocity) * 0.5f;
    p_rigidBody.m_dP = p_rigidBody.m_force;
    p_rigidBody.m_dL = p_rigidBody.m_torque;
    p_rigidBody.m_quatDt = 0.5f * (glm::quat(p_rigidBody.m_angularVelocity) * p_rigidBody.m_quat);
}

void PhysicsManager::collisionChecker(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    m_collsionHandler.collisionChecker(p_object, p_objects);
}

}//Physics
}//McEngine
