#include "CollisionHandler.h"
#include "CubeAABBCollisionHandler.h"
#include "CubeOBBCollsionHandler.h"
#include "SphereCollsionHandler.h"
#include "SphereVAABBCollsionHandler.h"
#include <algorithm>
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

    if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
        and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        m_meshCollisionHandler = std::make_unique<CubeOBBCollsionHandler>();
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    else if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB)

    {
        m_meshCollisionHandler = std::make_unique<CubeAABBCollisionHandler>();
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    else if (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
              and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        
        m_meshCollisionHandler = std::make_unique<SphereCollsionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    else if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
             or 
             (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB))
    {
        m_meshCollisionHandler = std::make_unique<SphereVAABBCollsionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }
    else if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
             and p_ColliderB.m_colliderType == Meshes::ColliderType::SPHERE)
             or 
             (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
             and p_ColliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB))
    {
        //m_meshCollisionHandler = std::make_unique<SphereVCubeOBBCollsionHandler>();
        //return m_meshCollisionHandler->checkCollision(p_colliderA, p_ColliderB);
    }

    return false;
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
                if (checkCollsionForObject(colliderA, 
                                           colliderB, 
                                           p_object.m_transform.m_position, 
                                           p_objects[i].first.m_transform.m_position))
                {
                    colliderA.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
                    l_isColliding = true;
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