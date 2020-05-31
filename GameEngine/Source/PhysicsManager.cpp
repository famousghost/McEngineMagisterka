#include "PhysicsManager.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{

void PhysicsManager::start()
{
    m_shouldCheckCollsion = false;
}

void PhysicsManager::shutdown()
{
}

PhysicsManager & PhysicsManager::getInstance()
{
    static PhysicsManager l_physicsManager;
    return l_physicsManager;
}

bool PhysicsManager::checkCollisionAABB(const Meshes::Colider& p_coliderA, 
                                        const Meshes::Colider& p_coliderB)
{
    return (p_coliderA.m_xSection.min.x <= p_coliderB.m_xSection.max.x
           and p_coliderA.m_xSection.max.x >= p_coliderB.m_xSection.min.x)
           and (p_coliderA.m_ySection.min.y <= p_coliderB.m_ySection.max.y
           and p_coliderA.m_ySection.max.y >= p_coliderB.m_ySection.min.y)
           and (p_coliderA.m_zSection.min.z >= p_coliderB.m_zSection.max.z
           and p_coliderA.m_zSection.max.z <= p_coliderB.m_zSection.min.z);
}

std::vector<double> PhysicsManager::getProjectedPointsToAxis(const std::vector<glm::vec4>& p_listOfVec,
                                                             const glm::vec3& p_normals)
{
    std::vector<double> l_result;
    for (const auto& vec : p_listOfVec)
    {
        auto value = glm::dot(glm::vec3(vec), p_normals);

        l_result.push_back(value);
    }
    return l_result;
}
bool PhysicsManager::checkCollisionForAxis(const std::vector<double>& p_projectedColliderA, 
                                           const std::vector<double>& p_projectedColliderB)
{
    auto minA = std::min_element(p_projectedColliderA.begin(), p_projectedColliderA.end());
    auto maxA = std::max_element(p_projectedColliderA.begin(), p_projectedColliderA.end());

    auto minB = std::min_element(p_projectedColliderB.begin(), p_projectedColliderB.end());
    auto maxB = std::max_element(p_projectedColliderB.begin(), p_projectedColliderB.end());
    
    auto l_longSpan = std::max(*maxA, *maxB) - std::min(*minA, *minB);
    auto l_sumSpan = *maxA - *minA + *maxB - *minB;
    return l_longSpan <= l_sumSpan;
}

bool PhysicsManager::checkCollisionForNormalAxis(const std::vector<double>& p_projectedColliderAX,
                                                 const std::vector<double>& p_projectedColliderAY,
                                                 const std::vector<double>& p_projectedColliderAZ,
                                                 const std::vector<double>& p_projectedColliderBX,
                                                 const std::vector<double>& p_projectedColliderBY,
                                                 const std::vector<double>& p_projectedColliderBZ)
{
    auto minAX = std::min_element(p_projectedColliderAX.begin(), p_projectedColliderAX.end());
    auto minAY = std::min_element(p_projectedColliderAY.begin(), p_projectedColliderAY.end());
    auto minAZ = std::max_element(p_projectedColliderAZ.begin(), p_projectedColliderAZ.end());

    auto maxAX = std::max_element(p_projectedColliderAX.begin(), p_projectedColliderAX.end());
    auto maxAY = std::max_element(p_projectedColliderAY.begin(), p_projectedColliderAY.end());
    auto maxAZ = std::min_element(p_projectedColliderAZ.begin(), p_projectedColliderAZ.end());

    auto minBX = std::min_element(p_projectedColliderBX.begin(), p_projectedColliderBX.end());
    auto minBY = std::min_element(p_projectedColliderBY.begin(), p_projectedColliderBY.end());
    auto minBZ = std::max_element(p_projectedColliderBZ.begin(), p_projectedColliderBZ.end());

    auto maxBX = std::max_element(p_projectedColliderBX.begin(), p_projectedColliderBX.end());
    auto maxBY = std::max_element(p_projectedColliderBY.begin(), p_projectedColliderBY.end());
    auto maxBZ = std::min_element(p_projectedColliderBZ.begin(), p_projectedColliderBZ.end());

    auto l_longSpan = std::max(*maxAX, *maxBX) - std::min(*minAX, *minBX);
    auto l_sumSpan = *maxAX - *minAX + *maxBX - *minBX;

    if (l_longSpan > l_sumSpan)
    {
        return false;
    }

    l_longSpan = std::max(*maxAY, *maxBY) - std::min(*minAY, *minBY);
    l_sumSpan = *maxAY - *minAY + *maxBY - *minBY;

    if (l_longSpan > l_sumSpan)
    {
        return false;
    }

    l_longSpan = std::min(*maxAZ, *maxBZ) - std::max(*minAZ, *minBZ);
    l_sumSpan = *minAZ - *maxAZ + *minBZ - *maxBZ;

    if (l_longSpan > l_sumSpan)
    {
        return false;
    }

    return true;
}

bool PhysicsManager::checkCollisionOBB(const Meshes::Colider & p_coliderA, 
                                       const Meshes::Colider & p_coliderB)
{
    std::vector<glm::vec3> l_colliderAEdges;
    std::vector<glm::vec3> l_colliderBEdges;

    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.x));
    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.y));
    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.z));

    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.x));
    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.y));
    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.z));

    auto l_projectedColiderAToAX = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[0]);
    auto l_projectedColiderAToAY = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[1]);
    auto l_projectedColiderAToAZ = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[2]);

    auto l_projectedColiderAToBX = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[0]);
    auto l_projectedColiderAToBY = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[1]);
    auto l_projectedColiderAToBZ = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[2]);

    auto l_projectedColiderBToBX = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[0]);
    auto l_projectedColiderBToBY = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[1]);
    auto l_projectedColiderBToBZ = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[2]);

    auto l_projectedColiderBToAX = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[0]);
    auto l_projectedColiderBToAY = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[1]);
    auto l_projectedColiderBToAZ = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[2]);

    if (not checkCollisionForNormalAxis(l_projectedColiderAToAX,
        l_projectedColiderAToAY,
        l_projectedColiderAToAZ,
        l_projectedColiderBToAX,
        l_projectedColiderBToAY,
        l_projectedColiderBToAZ))
    {
        return false;
    }

    if (not checkCollisionForNormalAxis(l_projectedColiderAToBX,
        l_projectedColiderAToBY,
        l_projectedColiderAToBZ,
        l_projectedColiderBToBX,
        l_projectedColiderBToBY,
        l_projectedColiderBToBZ))
    {
        return false;
    }

    for (const auto& vertexA : l_colliderAEdges)
    {
        for (const auto& vertexB : l_colliderBEdges)
        {
            auto l_normal = glm::cross(vertexA, vertexB);

            if (l_normal == glm::vec3())
            {
                continue;
            }

            auto l_projectedColiderA = getProjectedPointsToAxis(p_coliderA.m_verticies, l_normal);
            auto l_projectedColiderB = getProjectedPointsToAxis(p_coliderB.m_verticies, l_normal);

            if (not checkCollisionForAxis(l_projectedColiderA,
                                          l_projectedColiderB))
            {
                return false;
            }
        }
    }

    return true;
}

void PhysicsManager::print(const glm::vec4& p_vec)
{
    std::cout << "(" << p_vec.x << ", " << p_vec.y << ", " << p_vec.z << ")" << std::endl;
}

bool PhysicsManager::getShouldCheckCollsion() const
{
    return m_shouldCheckCollsion;
}

void PhysicsManager::setShouldCheckCollision(bool p_shouldCheckCollision)
{
    m_shouldCheckCollsion = p_shouldCheckCollision;
}

void PhysicsManager::checkCollisionDetectionAABB(Meshes::Object& p_object, 
                                                 std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    if(m_shouldCheckCollsion)
    {
        collisionCheckerAABB(p_object, p_objects);
    }
}

void PhysicsManager::checkCollisionDetectionOBB(Meshes::Object & p_object, 
                                                std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
   // if (m_shouldCheckCollsion)
   // {
        collisionCheckerOBB(p_object, p_objects);
   // }
}

void PhysicsManager::collisionCheckerOBB(Meshes::Object& p_object,
                                         std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool l_isColliding = false;
    for (std::size_t j = 0; j < p_objects.size(); ++j)
    {
        if (p_object.m_objectName == p_objects[j].first.m_objectName)
        {
            continue;
        }

        if (checkCollisionOBB(p_object.m_colider, p_objects[j].first.m_colider))
        {
            p_object.m_colider.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
            l_isColliding = true;
        }
    }

    if(not l_isColliding)
    {
        p_object.m_colider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void PhysicsManager::collisionCheckerAABB(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool l_isColliding = false;
    for (std::size_t j = 0; j < p_objects.size(); ++j)
    {
        if (p_object.m_objectName == p_objects[j].first.m_objectName)
        {
            continue;
        }

        if (checkCollisionAABB(p_object.m_colider, p_objects[j].first.m_colider))
        {
            p_object.m_colider.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
        }
    }
    if (not l_isColliding)
    {
        p_object.m_colider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }

}

}//Physics
}//McEngine
