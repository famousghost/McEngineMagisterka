#include "CollisionHandler.h"
#include "CubeAABBCollisionHandler.h"
#include "CubeOBBCollsionHandler.h"
#include "SphereCollsionHandler.h"
#include "SphereVAABBCollsionHandler.h"
#include "SphereVOBBCollisionHandler.h"
#include <algorithm>
#include "TimeManager.h"
#include <math.h>

namespace McEngine
{
namespace Physics
{
CollisionHandler::CollisionHandler()
    :m_shouldCheckCollsion(false)
{
}

bool CollisionHandler::getShouldCheckCollsion() const
{
    return m_shouldCheckCollsion;
}

void CollisionHandler::setShouldCheckCollision(bool p_shouldCheckCollision)
{
    m_shouldCheckCollsion = p_shouldCheckCollision;
}

bool CollisionHandler::checkCollsionForObject(const Meshes::Collider& p_colliderA,
                                              const Meshes::Collider& p_ColliderB,
                                              const glm::vec3& p_objectCenterA,
                                              const glm::vec3& p_objectCenterB)
{
    std::unique_ptr<MeshCollisionHandler> m_meshCollisionHandler;

    m_collsionDirection = glm::normalize(p_objectCenterB - p_objectCenterA);
    if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
        and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        m_meshCollisionHandler = std::make_unique<CubeOBBCollsionHandler>();
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB)

    {
        m_meshCollisionHandler = std::make_unique<CubeAABBCollisionHandler>();
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);;
    }
    if (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
              and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        
        m_meshCollisionHandler = std::make_unique<SphereCollsionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
             or 
             (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB))
    {
        m_meshCollisionHandler = std::make_unique<SphereVAABBCollsionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
             or 
             (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB))
    {
        m_meshCollisionHandler = std::make_unique<SphereVOBBCollisionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
}

void CollisionHandler::collisionChecker(Meshes::Object& p_object,
                                        std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool l_isColliding = false;
    for (std::size_t i = 0; i < p_objects.size(); ++i)
    {
        if (p_object.m_objectName == p_objects[i].first.m_objectName)
        {
            continue;
        }

        for(auto& colliderA : p_object.m_colider)
        {
            for(auto& colliderB : p_objects[i].first.m_colider)
            { 
                if (p_object.m_isColliding = checkCollsionForObject(colliderA, 
                                                                    colliderB, 
                                                                    p_object.m_transform.m_position, 
                                                                    p_objects[i].first.m_transform.m_position))
                {
                    colliderA.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
                    auto& l_timeManager = Time::TimeManager::getInstance();
                    if (p_object.m_rigidBody)
                    {
                        p_object.m_gravity = 0.0f;
                    }
                    p_object.m_transform.m_position -= 0.5f * p_object.m_velocity * m_collsionDirection * static_cast<float>(l_timeManager.getDeltaTime());
                    m_collsionDirection = glm::vec3();
                    l_isColliding = true;
                }
                else
                {
                    if (p_object.m_rigidBody)
                    {
                        p_object.m_gravity = 9.81f;
                    }
                }
            }
        }
    }

    if (not l_isColliding)
    {
        for(auto& collider : p_object.m_colider)
        {
            collider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    }
}

}//Physics
}//McEngine