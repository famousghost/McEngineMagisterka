#include "RigidbodyHandlerOBB.h"

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
    m_rigidbody->m_force = m_forces;
    //solveConstraints();
}

void RigidbodyHandlerOBB::solveConstraints(const std::vector<std::pair<Meshes::Object, std::string>>& p_constraintObjects)
{
}

void RigidbodyHandlerOBB::applyForces()
{
    applyGravityForce();
}

void RigidbodyHandlerOBB::applyGravityForce()
{
    if (m_rigidbody->m_gravityForce)
    {
        m_forces.y = m_rigidbody->m_gravity * m_rigidbody->m_massProperties.m_mass;
    }
}

}
}