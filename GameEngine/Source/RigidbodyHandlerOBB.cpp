#include "RigidbodyHandlerOBB.h"
#include "TimeManager.h"
#include "GuiManager.h"
#include "Ray.h"

namespace McEngine
{
namespace Physics
{

RigidbodyHandlerOBB::RigidbodyHandlerOBB(Meshes::Rigidbody * p_rigidbody)
    :m_rigidbody(p_rigidbody)
{
}

RigidbodyHandlerOBB::~RigidbodyHandlerOBB()
{
}

void RigidbodyHandlerOBB::update()
{
    applyForces();
    linearVelocity();
    angularVelocity();
}

void RigidbodyHandlerOBB::linearVelocity()
{
    const float l_damping = 0.98f;
    glm::vec3 l_acceleration = (m_rigidbody->m_force + m_forces) * m_rigidbody->m_massProperties.m_inverseMass;
    m_rigidbody->m_velocity += 
        l_acceleration * static_cast<float>(Time::TimeManager::getInstance().getDeltaTime());
    m_rigidbody->m_velocity *= l_damping;
}

void RigidbodyHandlerOBB::angularVelocity()
{
    const float l_damping = 0.98f;
    glm::vec3 l_angularAcceleration = m_rigidbody->m_torque * glm::mat3(m_rigidbody->m_invBodyTensorOfInteria);
    m_rigidbody->m_angularVelocity += 
        l_angularAcceleration * static_cast<float>(Time::TimeManager::getInstance().getDeltaTime());
    m_rigidbody->m_angularVelocity *= l_damping;
}

void RigidbodyHandlerOBB::solveConstraints(const std::vector<std::pair<Meshes::Object, std::string>>& p_constraintObjects)
{
}

void RigidbodyHandlerOBB::applyForces()
{
    m_forces += gravityForce();
}

glm::vec3 RigidbodyHandlerOBB::gravityForce()
{
    if (m_rigidbody->m_gravityForce)
    {
        return glm::vec3(0.0f, -9.82f, 0.0f) * m_rigidbody->m_massProperties.m_mass;
    }
    return glm::vec3();
}

}
}