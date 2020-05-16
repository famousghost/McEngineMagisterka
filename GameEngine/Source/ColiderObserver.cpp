#include "ColiderObserver.h"
#include "PhysicsManager.h"

namespace McEngine
{
namespace Meshes
{

ColiderObserver::ColiderObserver(Object & p_object)
    :m_object(p_object)
{
    m_object.attach(this);
    m_transform = m_object.m_transform;
}

void ColiderObserver::update(const Transform& p_transformation)
{
    auto& l_physicsManager = Physics::PhysicsManager::getInstance();
    if (m_transform != p_transformation)
    {
        m_transform = p_transformation;
        l_physicsManager.setShouldCheckCollision(true);
        return;
    }
    l_physicsManager.setShouldCheckCollision(false);
}

}//Meshs
}//McEngine