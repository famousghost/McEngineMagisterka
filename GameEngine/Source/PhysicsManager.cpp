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

std::vector<double> PhysicsManager::getProjectedPointsToAxisX(const std::vector<glm::vec4>& p_listOfVec,
                                                              const Meshes::Normals& p_normals)
{
    std::vector<double> l_result;
    for (const auto& vec : p_listOfVec)
    {
        auto v = glm::normalize(vec);
        auto normalX = glm::normalize(p_normals.x);
        auto x = glm::dot(v, normalX);

        l_result.push_back(x);
    }
    return l_result;
}

std::vector<double> PhysicsManager::getProjectedPointsToAxisY(const std::vector<glm::vec4>& p_listOfVec,
                                                              const Meshes::Normals& p_normals)
{
    std::vector<double> l_result;
    for (const auto& vec : p_listOfVec)
    {
        auto v = glm::normalize(vec);
        auto normalY = glm::normalize(p_normals.y);
        auto y = glm::dot(v, normalY);

        l_result.push_back(y);
    }
    return l_result;
}

std::vector<double> PhysicsManager::getProjectedPointsToAxisZ(const std::vector<glm::vec4>& p_listOfVec,
                                                              const Meshes::Normals& p_normals)
{
    std::vector<double> l_result;
    for (const auto& vec : p_listOfVec)
    {
        auto v = glm::normalize(vec);
        auto normalZ = glm::normalize(p_normals.z);
        auto z = glm::dot(v, normalZ);

        l_result.push_back(z);
    }
    return l_result;
}

bool PhysicsManager::checkCollisionForAxis(const std::vector<double>& p_projectedColliderAX, 
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

    if (*minAX <= *maxBX
        and (*maxAX >= *minBX)
        and (*minAY <= *maxBY)
        and (*maxAY >= *minBY)
        and (*minAZ >= *maxBZ)
        and (*maxAZ <= *minBZ))
    {
        return true;
    }
    return false;
}

bool PhysicsManager::checkCollisionOBB(const Meshes::Colider & p_coliderA, 
                                       const Meshes::Colider & p_coliderB)
{
    bool result = false;

    auto l_projectedColiderAToAX = getProjectedPointsToAxisX(p_coliderA.m_verticies, p_coliderA.m_normals);
    auto l_projectedColiderAToAY = getProjectedPointsToAxisY(p_coliderA.m_verticies, p_coliderA.m_normals);
    auto l_projectedColiderAToAZ = getProjectedPointsToAxisZ(p_coliderA.m_verticies, p_coliderA.m_normals);

    auto l_projectedColiderAToBX = getProjectedPointsToAxisX(p_coliderA.m_verticies, p_coliderB.m_normals);
    auto l_projectedColiderAToBY = getProjectedPointsToAxisY(p_coliderA.m_verticies, p_coliderB.m_normals);
    auto l_projectedColiderAToBZ = getProjectedPointsToAxisZ(p_coliderA.m_verticies, p_coliderB.m_normals);

    auto l_projectedColiderBToBX = getProjectedPointsToAxisX(p_coliderB.m_verticies, p_coliderB.m_normals);
    auto l_projectedColiderBToBY = getProjectedPointsToAxisY(p_coliderB.m_verticies, p_coliderB.m_normals);
    auto l_projectedColiderBToBZ = getProjectedPointsToAxisZ(p_coliderB.m_verticies, p_coliderB.m_normals);

    auto l_projectedColiderBToAX = getProjectedPointsToAxisX(p_coliderB.m_verticies, p_coliderA.m_normals);
    auto l_projectedColiderBToAY = getProjectedPointsToAxisY(p_coliderB.m_verticies, p_coliderA.m_normals);
    auto l_projectedColiderBToAZ = getProjectedPointsToAxisZ(p_coliderB.m_verticies, p_coliderA.m_normals);

    result = checkCollisionForAxis(l_projectedColiderAToAX, 
                                   l_projectedColiderAToAY,
                                   l_projectedColiderAToAZ,
                                   l_projectedColiderBToAX,
                                   l_projectedColiderBToAY,
                                   l_projectedColiderBToAZ);

    result = checkCollisionForAxis(l_projectedColiderAToBX,
                                   l_projectedColiderAToBY,
                                   l_projectedColiderAToBZ,
                                   l_projectedColiderBToBX,
                                   l_projectedColiderBToBY,
                                   l_projectedColiderBToBZ);
    return result;
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
    if (m_shouldCheckCollsion)
    {
        collisionCheckerOBB(p_object, p_objects);
    }
}

void PhysicsManager::collisionCheckerOBB(Meshes::Object& p_object,
                                         std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool isColliding = false;
    for (std::size_t j = 0; j < p_objects.size(); ++j)
    {
        if (p_object.m_objectName == p_objects[j].first.m_objectName)
        {
            continue;
        }

        if (checkCollisionOBB(p_object.m_colider, p_objects[j].first.m_colider))
        {
            p_object.m_colider.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
            isColliding = true;
        }
    }

    if (not isColliding)
    {
        p_object.m_colider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

void PhysicsManager::collisionCheckerAABB(Meshes::Object& p_object,
                                      std::vector<std::pair<Meshes::Object, std::string>>& p_objects)
{
    bool isColliding = false;
    for (std::size_t j = 0; j < p_objects.size(); ++j)
    {
        if (p_object.m_objectName == p_objects[j].first.m_objectName)
        {
            continue;
        }

        if (checkCollisionAABB(p_object.m_colider, p_objects[j].first.m_colider))
        {
            p_object.m_colider.m_coliderColor = glm::vec3(1.0f, 0.0f, 0.0f);
            isColliding = true;
        }
    }
    if (not isColliding)
    {
        p_object.m_colider.m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
    }
}

}//Physics
}//McEngine
