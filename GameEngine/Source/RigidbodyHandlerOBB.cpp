#include "RigidbodyHandlerOBB.h"
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
    m_forces = glm::vec3();
    applyForces();
    m_rigidbody->m_force = m_forces;
    //solveConstraints();
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
    if (m_rigidbody->m_isOnGrounded)
    {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }
    if (m_rigidbody->m_gravityForce)
    {
        return glm::vec3(0.0f, -9.82f, 0.0f);
    }
    return glm::vec3();
}

}
}