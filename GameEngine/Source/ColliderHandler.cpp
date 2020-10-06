#include "CollisionHandler.h"
#include "CubeAABBCollisionHandler.h"
#include "CubeOBBCollsionHandler.h"
#include "SphereCollsionHandler.h"
#include "SphereVAABBCollsionHandler.h"
#include "SphereVOBBCollisionHandler.h"
#include <algorithm>
#include "TimeManager.h"
#include "Geometry3dUtils.h"
#include "RigidbodyHandlerOBB.h"
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

glm::vec3 CollisionHandler::calculateRealCollsionDirection(const glm::vec3& p_collsionDirection)
{
    glm::vec3 l_realDirectionResult;
    std::vector<int> l_indicies;
    std::vector<float> l_directions = {fabs(p_collsionDirection.x), 
                                       fabs(p_collsionDirection.y), 
                                       fabs(p_collsionDirection.z)};
    float l_maxElem = 0.0f;
    for(std::size_t i = 0; i < 3; ++i)
    {
        auto l_maxElementIt = std::max_element(l_directions.begin(), l_directions.end());
        int l_maxElemIndex = std::distance(l_directions.begin(), l_maxElementIt);
        if (l_maxElem <= *l_maxElementIt)
        {
            l_maxElem = *l_maxElementIt;
            l_indicies.push_back(l_maxElemIndex);
            *l_maxElementIt = 0.0f;
        }
    }

    for (std::size_t i = 0; i < l_indicies.size(); ++i)
    {
        if (l_indicies[i] == 0)
        {
            l_realDirectionResult.x = p_collsionDirection.x;
        }
        else if (l_indicies[i] == 1)
        {
            l_realDirectionResult.y = p_collsionDirection.y;
        }
        else if (l_indicies[i] == 2)
        {
            l_realDirectionResult.z = p_collsionDirection.z;
        }
    }

    return l_realDirectionResult;
}

bool CollisionHandler::checkCollsionForObject(const Meshes::Collider& p_colliderA,
                                              const Meshes::Collider& p_colliderB,
                                              const glm::vec3& p_objectCenterA,
                                              const glm::vec3& p_objectCenterB,
                                              const Meshes::Object* p_objectA,
                                              const Meshes::Object* p_objectB)
{
    std::unique_ptr<MeshCollisionHandler> m_meshCollisionHandler;

    m_collsionDirection = -calculateRealCollsionDirection(glm::normalize(p_objectCenterB - p_objectCenterA));

    if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
        and p_colliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        m_meshCollisionHandler = std::make_unique<CubeOBBCollsionHandler>(m_collsionDirection, 
                                                                          p_objectCenterA, 
                                                                          p_objectCenterB,
                                                                          p_objectA,
                                                                          p_objectB);
        bool l_result = m_meshCollisionHandler->checkCollision(p_colliderA, p_colliderB);
        m_colMainfold = m_meshCollisionHandler->getColMainfold();

        return l_result;
    }
    if (p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_colliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB)

    {
        m_meshCollisionHandler = std::make_unique<CubeAABBCollisionHandler>();
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_colliderB);;
    }
    if (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
              and p_colliderB.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        
        m_meshCollisionHandler = std::make_unique<SphereCollsionHandler>(p_objectCenterA, p_objectCenterB);
        bool l_result = m_meshCollisionHandler->checkCollision(p_colliderA, p_colliderB);
        m_colMainfold = m_meshCollisionHandler->getColMainfold();
        return l_result;
    }
    if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_AABB
             and p_colliderB.m_colliderType == Meshes::ColliderType::SPHERE)
             or 
             (p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
             and p_colliderB.m_colliderType == Meshes::ColliderType::CUBE_AABB))
    {
        m_meshCollisionHandler = std::make_unique<SphereVAABBCollsionHandler>(p_objectCenterA, p_objectCenterB);
        return m_meshCollisionHandler->checkCollision(p_colliderA, p_colliderB);
    }
    if ((p_colliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB
         and p_colliderB.m_colliderType == Meshes::ColliderType::SPHERE))
    {
        m_meshCollisionHandler = std::make_unique<SphereVOBBCollisionHandler>(p_objectCenterA, 
                                                                              p_objectCenterB, 
                                                                              m_collsionDirection,
                                                                              p_objectA);
        bool l_result = m_meshCollisionHandler->checkCollision(p_colliderA, p_colliderB);
        m_colMainfold = m_meshCollisionHandler->getColMainfold();
        return l_result;
    }
    if ((p_colliderA.m_colliderType == Meshes::ColliderType::SPHERE
         and p_colliderB.m_colliderType == Meshes::ColliderType::CUBE_OBB))
    {
        m_meshCollisionHandler = std::make_unique<SphereVOBBCollisionHandler>(p_objectCenterB,
                                                                              p_objectCenterA,
                                                                              m_collsionDirection,
                                                                              p_objectB);
        bool l_result = m_meshCollisionHandler->checkCollision(p_colliderB, p_colliderA);
        m_colMainfold = m_meshCollisionHandler->getColMainfold();
        return l_result;
    }
    return false;
}

void CollisionHandler::collisionChecker(Meshes::Object& p_object,
                                        std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    if (not p_object.m_isRigidBody)
    {
        return;
    }
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
                                                                    p_objects[i].first.m_transform.m_position,
                                                                    &p_object,
                                                                    &p_objects[i].first))
                {
                    Meshes::Ray l_ray = Meshes::Ray(p_object.m_transform.m_position, glm::vec3(0.0f, -1.0f, 0.0f));
                    if (Utils::Geometry3dUtils::raycast(p_objects[i].first, l_ray))
                    {
                        p_object.m_rigidBody.m_isOnGrounded = true;
                    }
                    else
                    {
                        p_object.m_rigidBody.m_isOnGrounded = false;
                    }

                    colliderA.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);

                    std::cout << "Contact points: " << m_colMainfold.m_contacts.size() << std::endl;
                    p_object.m_transform.m_position -= m_colMainfold.m_normal * m_colMainfold.m_depth;

                    if(p_objects[i].first.m_isRigidBody)
                    {
                        p_objects[i].first.m_transform.m_position += m_colMainfold.m_normal * m_colMainfold.m_depth;
                    }

                    for (int j = 0; j < m_colMainfold.m_contacts.size(); ++j)
                    {
                        Utils::Geometry3dUtils::applyImpulse(p_object, p_objects[i].first, m_colMainfold, j);
                    }
                }
            }
        }
    }

    if (not p_object.m_isColliding)
    {
        for(auto& collider : p_object.m_colider)
        {
            collider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
        }
    }

}

}//Physics
}//McEngine